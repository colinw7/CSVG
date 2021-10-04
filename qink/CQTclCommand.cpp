#include <CQTclCommand.h>
#include <CQTclUtil.h>
#include <CQTclCmd.h>

#include <CTclParse.h>
#include <CQStrUtil.h>
#include <COSProcess.h>
#include <COSExec.h>

namespace CQTclCommand {

CommandWidget::
CommandWidget(ScrollArea *area) :
 CQCommand::CommandWidget(area)
{
  qtcl_ = new CQTcl;

  qtcl_->createAlias("echo", "puts");

  mgr_ = new CQTclCmd::Mgr(qtcl_);
}

CommandWidget::
~CommandWidget()
{
  delete mgr_;
  delete qtcl_;
}

CQTclCmd::Mgr *
CommandWidget::
tclCmdMgr() const
{
  return mgr_;
}

void
CommandWidget::
addCommand(const QString &name, CQTclCmd::CmdProc *proc)
{
  mgr_->addCommand(name, proc);
}

void
CommandWidget::
executeCommand(const QString &line)
{
  auto line1 = line.trimmed();

  if (! line1.length())
    return;

  if (line1[0] == '!') {
    line1 = line.mid(1);

    std::string str;

    COSProcess::executeCommand(line1.toStdString(), str);

    outputText(QString::fromStdString(str));

    return;
  }

  COSExec::grabOutput();

  bool log = true;

  int rc = eval(line, /*showError*/true, /*showResult*/log);

  std::cout << std::flush;

  if (rc != TCL_OK)
    std::cerr << "Invalid line: '" << line.toStdString() + "'\n";

  std::string str;

  COSExec::readGrabbedOutput(str);

  COSExec::ungrabOutput();

  outputText(QString::fromStdString(str));
}

int
CommandWidget::
eval(const QString &line, bool showError, bool showResult)
{
  return qtcl_->eval(line, showError, showResult);
}

void
CommandWidget::
textChanged()
{
  const auto &str = getText();

  CTclParse parse;

  CTclParse::Tokens tokens;

  parse.parseLine(str.toStdString(), tokens);

  colors_.resize(str.length());

  for (const auto *token : tokens) {
    int pos = token->pos();
    int len = token->str().size();

    QColor c;

    if      (token->type() == CTclToken::Type::COMMAND ||
             token->type() == CTclToken::Type::SUB_COMMAND)
      c = QColor(200, 100, 100);
    else if (token->type() == CTclToken::Type::LITERAL_STRING ||
             token->type() == CTclToken::Type::QUOTED_STRING ||
             token->type() == CTclToken::Type::STRING)
      c = QColor(100, 200, 100);
    else if (token->type() == CTclToken::Type::VARIABLE ||
             token->type() == CTclToken::Type::SEPARATOR)
      c = QColor(100, 100, 200);

    for (int i = 0; i < len; ++i)
      colors_[pos + i] = c;
  }
}

QColor
CommandWidget::
posColor(int pos) const
{
  return colors_[pos];
}

bool
CommandWidget::
complete(const QString &text, int pos, QString &newText, CompleteMode completeMode) const
{
  bool interactive = (completeMode == CompleteMode::Interactive);

  CTclParse parse;

  CTclParse::Tokens tokens;

  parse.parseLine(text.toStdString(), tokens);

  auto *token = parse.getTokenForPos(tokens, pos);

  if (! token)
    token = parse.getTokenForPos(tokens, pos - 1);

  //---

  auto lhs = text.mid(0, token ? token->pos() : pos + 1);
  auto str = (token ? token->str() : "");
  auto rhs = text.mid(token ? token->endPos() + 1 : pos + 1);

  //---

  std::string command;
  int         commandPos { -1 };
//bool        commandInside = false;
  std::string option;
  int         optionPos { -1 };
  bool        optionInside = false;

  // complete command
  if      (token && token->type() == CTclToken::Type::COMMAND) {
    command       = str;
    commandPos    = token->pos();
  //commandInside = true;
  }
  // complete option
  else if (str[0] == '-') {
    option       = str;
    optionPos    = token->pos();
    optionInside = true;

    // get previous token for command name
    for (int pos1 = pos - 1; pos1 >= 0; --pos1) {
      auto *token1 = parse.getTokenForPos(tokens, pos1);
      if (! token1) continue;

      const auto &str = token1->str();
      if (str.empty()) continue;

      if (token1->type() == CTclToken::Type::COMMAND) {
        command    = str;
        commandPos = token1->pos();
        break;
      }
    }
  }
  else {
    // get previous tokens for option name and command name
    for (int pos1 = pos - 1; pos1 >= 0; --pos1) {
      auto *token1 = parse.getTokenForPos(tokens, pos1);
      if (! token1) continue;

      const auto &str = token1->str();
      if (str.empty()) continue;

      if      (token1->type() == CTclToken::Type::COMMAND) {
        command    = str;
        commandPos = token1->pos();
        break;
      }
      else if (str[0] == '-') {
        if (option.empty()) {
          option    = str;
          optionPos = token1->pos();
        }

        if (pos1 > token1->pos())
          pos1 = token1->pos(); // move to start
      }
    }
  }

  // no command
  if (command == "")
    return false;

  // no option so complete command
  if (option == "") {
    auto lhs = text.mid(0, commandPos);

    return completeCommand(lhs, QString::fromStdString(command), rhs, interactive, newText);
  }

  // command and option (and in option) then complete option
  if (optionInside) {
    auto lhs = text.mid(0, optionPos);

    return completeOption(QString::fromStdString(command), lhs, QString::fromStdString(option),
                          rhs, interactive, newText);
  }

  // get option values to next command
  OptionValues optionValues;

  std::string lastOption;

  for (int pos1 = commandPos + command.length(); pos1 < text.length(); ++pos1) {
    auto *token1 = parse.getTokenForPos(tokens, pos1);
    if (! token1) continue;

    const auto &str = token1->str();
    if (str.empty()) continue;

    if      (token1->type() == CTclToken::Type::COMMAND) {
      break;
    }
    else if (str[0] == '-') {
      lastOption = str.substr(1);

      optionValues[lastOption] = "";

      pos1 = token1->pos() + token1->str().length(); // move to end
    }
    else {
      if (lastOption != "")
        optionValues[lastOption] = str;

      pos1 = token1->pos() + token1->str().length(); // move to end
    }
  }

  // complete arg
  return completeArg(QString::fromStdString(command), QString::fromStdString(option),
                     optionValues, lhs, "", rhs, interactive, newText);
}

bool
CommandWidget::
completeCommand(const QString &lhs, const QString &command, const QString &rhs,
                bool interactive, QString &newText) const
{
  const auto &cmds = qtcl_->commandNames();

  auto matchCmds = CQStrUtil::matchStrs(command, cmds);

  if (matchCmds.empty()) {
    QVariant res;

    qtcl_->eval("info commands", res, /*showError*/false);

    auto tclCmds = qtcl_->resToStrings(res);
    //(void) qtcl_->splitList(qtcl_->resToString(res), tclCmds);

    matchCmds = CQStrUtil::matchStrs(command, tclCmds);
  }

  QString matchStr;
  bool    exact = false;

  if      (matchCmds.size() == 1) {
    matchStr = matchCmds[0];
    exact    = true;
  }
  else if (matchCmds.size() > 1) {
    if (interactive) {
      matchStr = const_cast<CommandWidget *>(this)->showCompletionChooser(matchCmds);

      if (matchStr != "")
        exact = true;
    }
    else {
      matchStr = CQStrUtil::longestMatch(matchCmds, exact);
    }
  }

  //---

  newText = lhs;

  if (matchStr != "")
    newText += matchStr;

  if (exact)
    newText += " ";

  newText += rhs;

  return (matchStr.length() > command.length());
}

bool
CommandWidget::
completeOption(const QString &command, const QString &lhs, const QString &option,
               const QString &rhs, bool interactive, QString &newText) const
{
  auto *cmd = tclCmdMgr()->getCommand(command);
  if (! cmd) return false;

  CQTclCmd::Mgr::Vars vars;

  auto *args = tclCmdMgr()->createArgs(command, vars);

  cmd->addArgs(*args);

  QStringList names;

  for (const auto &arg : args->cmdArgs()) {
    names.push_back("-" + arg.name());
  }

  delete args;

  auto matchNames = CQStrUtil::matchStrs(option, names);

  QString matchStr;
  bool    exact = false;

  if      (matchNames.size() == 1) {
    matchStr = matchNames[0];
    exact    = true;
  }
  else if (matchNames.size() > 1) {
    if (interactive) {
      matchStr = const_cast<CommandWidget *>(this)->showCompletionChooser(matchNames);

      if (matchStr != "")
        exact = true;
    }
    else {
      matchStr = CQStrUtil::longestMatch(matchNames, exact);
    }
  }

  //---

  newText = lhs;

  if (matchStr != "")
    newText += matchStr;

  if (exact)
    newText += " ";

  newText += rhs;

  return (matchStr.length() > option.length());
}

bool
CommandWidget::
completeArg(const QString &command, const QString &option, const OptionValues &optionValues,
            const QString &lhs, const QString &arg, const QString &rhs,
            bool interactive, QString &newText) const
{
  auto *cmd = tclCmdMgr()->getCommand(command);
  if (! cmd) return false;

  CQTclCmd::Mgr::Vars vars;

  auto *args = tclCmdMgr()->createArgs(command, vars);

  cmd->addArgs(*args);

  CQTclCmd::CmdProc::NameValueMap nameArgValues;

  for (const auto &nv : optionValues)
    nameArgValues[QString::fromStdString(nv.first)] = QString::fromStdString(nv.second);

  nameArgValues[option] = arg;

  auto names = cmd->getArgValues(option, nameArgValues);

  delete args;

  auto matchNames = CQStrUtil::matchStrs(arg, names);

  QString matchStr;
  bool    exact = false;

  if      (matchNames.size() == 1) {
    matchStr = matchNames[0];
    exact    = true;
  }
  else if (matchNames.size() > 1) {
    if (interactive) {
      matchStr = const_cast<CommandWidget *>(this)->showCompletionChooser(matchNames);

      if (matchStr != "")
        exact = true;
    }
    else {
      matchStr = CQStrUtil::longestMatch(matchNames, exact);
    }
  }

  //---

  newText = lhs;

  if (matchStr != "")
    newText += matchStr;

  if (exact)
    newText += " ";

  newText += rhs;

  return (matchStr.length() > arg.length());
}

//---

ScrollArea::
ScrollArea(QWidget *parent) :
 CQCommand::ScrollArea(parent)
{
}

CQCommand::CommandWidget *
ScrollArea::
createCommandWidget() const
{
  auto *th = const_cast<ScrollArea *>(this);

  th->widget_ = new CommandWidget(th);

  return widget_;
}

CQTclCmd::Mgr *
ScrollArea::
tclCmdMgr() const
{
  return widget_->tclCmdMgr();
}

void
ScrollArea::
executeCommand(const QString &line)
{
  widget_->executeCommand(line);
}

int
ScrollArea::
eval(const QString &line, bool showError, bool showResult)
{
  return widget_->eval(line, showError, showResult);
}

void
ScrollArea::
addCommand(const QString &name, CQTclCmd::CmdProc *proc)
{
  widget_->addCommand(name, proc);
}

//---

}
