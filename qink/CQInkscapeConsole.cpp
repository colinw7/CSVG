#include <CQInkscapeConsole.h>
#include <CQInkscapeWindow.h>
#include <CQInkscapeCanvas.h>

#include <CQCommand.h>
#include <CQTclCmd.h>
#include <CQTclUtil.h>
#include <CQUtil.h>
#include <CTclParse.h>
#include <CQStrUtil.h>

#include <CSVG.h>

#include <COSProcess.h>
#include <COSExec.h>

#include <QVBoxLayout>

#include <fstream>

namespace CQInkscape {

class ConsoleCommandWidget : public CQCommand::CommandWidget {
 public:
  ConsoleCommandWidget(Console *console) :
   CommandWidget(nullptr), console_(console) {
  }

  Console *console() const { return console_; }

  bool complete(const QString &text, int pos, QString &newText,
                CompleteMode completeMode) const override {
    bool interactive = (completeMode == CompleteMode::Interactive);

    return console_->complete(const_cast<ConsoleCommandWidget *>(this), text,
                              pos, newText, interactive);

    return false;
  }

 private:
  Console *console_ { nullptr };
};

class ConsoleScrollArea : public CQCommand::ScrollArea {
 public:
  ConsoleScrollArea(Console *console) :
   console_(console) {
  }

  Console *console() const { return console_; }

  CQCommand::CommandWidget *createCommandWidget() const override {
    return new ConsoleCommandWidget(console_);
  }

 private:
  Console *console_ { nullptr };
};

Console::
Console(Window *window) :
 window_(window)
{
  setObjectName("console");

  qtcl_ = new CQTcl;

  qtcl_->createAlias("echo", "puts");

  mgr_ = new CQTclCmd::Mgr(qtcl_);

  mgr_->addCommand("help", new HelpTclCmd(this));

  mgr_->addCommand("get_svg_children" , new GetSVGChildrenTclCmd (this));
  mgr_->addCommand("get_svg_data"     , new GetSVGDataTclCmd     (this));
  mgr_->addCommand("set_svg_data"     , new SetSVGDataTclCmd     (this));
  mgr_->addCommand("add_svg_object"   , new AddSVGObjectTclCmd   (this));
  mgr_->addCommand("remove_svg_object", new RemoveSVGObjectTclCmd(this));
  mgr_->addCommand("get_svg_selection", new GetSVGSelectionTclCmd(this));
  mgr_->addCommand("move_svg_object"  , new MoveSVGObjectTclCmd  (this));
  mgr_->addCommand("scale_svg_object" , new ScaleSVGObjectTclCmd (this));
  mgr_->addCommand("rotate_svg_object", new RotateSVGObjectTclCmd(this));
  mgr_->addCommand("write_svg_file"   , new WriteSVGFileTclCmd   (this));

  //---

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  command_ = new ConsoleScrollArea(this);

  command_->getCommand()->setMinLines(10);
  command_->getCommand()->setPrompt("> ");

  connect(command_, SIGNAL(executeCommand(const QString &)),
          this, SLOT(executeCommand(const QString &)));

  layout->addWidget(command_);
}

Console::
~Console()
{
  delete mgr_;
  delete qtcl_;
}

void
Console::
executeCommand(const QString &line)
{
  auto line1 = line.trimmed();

  if (! line1.length())
    return;

  if (line1[0] == '!') {
    line1 = line.mid(1);

    std::string str;

    COSProcess::executeCommand(line1.toStdString(), str);

    command_->outputText(QString::fromStdString(str));

    return;
  }

  COSExec::grabOutput();

  bool log = true;

  int rc = qtcl_->eval(line, /*showError*/true, /*showResult*/log);

  std::cout << std::flush;

  if (rc != TCL_OK)
    std::cerr << "Invalid line: '" << line.toStdString() + "'\n";

  std::string str;

  COSExec::readGrabbedOutput(str);

  COSExec::ungrabOutput();

  command_->outputText(QString::fromStdString(str));
}

bool
Console::
complete(ConsoleCommandWidget *widget, const QString &text, int pos,
         QString &newText, bool interactive) const
{
  CTclParse parse;

  CTclParse::Tokens tokens;

  parse.parseLine(text.toStdString(), tokens);

  auto *token = parse.getTokenForPos(tokens, pos);

  //---

  auto completeCommand = [&](const QString &lhs, const QString &str, const QString &rhs,
                             QString &command) {
    const auto &cmds = qtcl_->commandNames();

    auto matchCmds = CQStrUtil::matchStrs(str, cmds);

    QString matchStr;
    bool    exact = false;

    if      (matchCmds.size() == 1) {
       matchStr = matchCmds[0];
       exact    = true;
    }
    else if (matchCmds.size() > 1) {
      if (interactive) {
        matchStr = widget->showCompletionChooser(matchCmds);

        if (matchStr != "")
          exact = true;
      }
      else {
        matchStr = CQStrUtil::longestMatch(matchCmds, exact);
      }
    }

    //---

    command = lhs;

    if (matchStr != "")
      command += matchStr;

    if (exact)
      command += " ";

    command += rhs;

    return (command.length() > text.length());
  };

  auto completeOption = [&](const QString &cmdName, const QString &lhs, const QString &str,
                            const QString &rhs, QString &option) {
    auto *cmd = tclCmdMgr()->getCommand(cmdName);
    if (! cmd) return false;

    CQTclCmd::Mgr::Vars vars;

    auto *args = tclCmdMgr()->createArgs(cmdName, vars);

    cmd->addArgs(*args);

    QStringList names;

    for (const auto &arg : args->cmdArgs()) {
      names.push_back(arg.name());
    }

    delete args;

    auto matchNames = CQStrUtil::matchStrs(str, names);

    bool exact;

    auto matchStr = CQStrUtil::longestMatch(matchNames, exact);

    option = lhs;

    if (matchStr != "")
      option += "-" + matchStr;

    if (exact)
      option += " ";

    option += rhs;

    return (option.length() > text.length());
  };

  //---

  auto lhs = text.mid(0, token ? token->pos() : pos + 1);
  auto str = (token ? token->str() : "");
  auto rhs = text.mid(token ? token->endPos() + 1 : pos + 1);

  // complete command
  if      (token && token->type() == CTclToken::Type::COMMAND) {
    //std::cerr << "Command: " << str << "\n";

    return completeCommand(lhs, QString::fromStdString(str), rhs, newText);
  }
  // complete option
  else if (str[0] == '-') {
    return false;
  }
  else {
    // get previous tokens for option name and command name
    using OptionValues = std::map<std::string, std::string>;

    std::string  command;
    int          commandPos { -1 };
    std::string  option;
    int          optionPos { -1 };
    OptionValues optionValues;

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
          option    = str.substr(1);
          optionPos = token1->pos();
        }

        if (pos1 > token1->pos())
          pos1 = token1->pos(); // move to start
      }
    }

    if (command == "")
      return false;

    if (option == "") {
      auto lhs = text.mid(0, commandPos);

      return completeCommand(lhs, QString::fromStdString(command), "", newText);
    }

    auto lhs = text.mid(0, optionPos);

    return completeOption(QString::fromStdString(command), lhs, QString::fromStdString(option), "",
                          newText);
  }
}

void
Console::
loadTclFile(const QString &file)
{
  executeCommand(QString("source %1").arg(file));
}

QSize
Console::
sizeHint() const
{
  return QSize(1000, 500);
}

//---

void
HelpTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-hidden" , ArgType::Boolean, "show hidden");
  addArg(argv, "-verbose", ArgType::Boolean, "verbose help");
}

QStringList
HelpTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
HelpTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  if (! argv.parse())
    return false;

  auto hidden  = argv.getParseBool("hidden");
  auto verbose = argv.getParseBool("verbose");

  //---

  const auto &pargs = argv.getParseArgs();

  QString pattern = (! pargs.empty() ? pargs[0].toString() : "");

  //---

  if (pattern.length())
    mgr_->help(pattern, verbose, hidden);
  else
    mgr_->helpAll(verbose, hidden);

  return true;
}

//---

void
GetSVGChildrenTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-parent", ArgType::String , "parent");
  addArg(argv, "-tag"   , ArgType::String , "tag name");
  addArg(argv, "-hier"  , ArgType::Boolean, "hierarchical");
}

QStringList
GetSVGChildrenTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
GetSVGChildrenTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto parent = argv.getParseStr ("parent");
  auto tag    = argv.getParseStr ("tag").toStdString();
  auto hier   = argv.getParseBool("hier");

  auto *svg = console_->window()->svg();

  CSVGObject *parentObj = nullptr;

  if (parent == "" || parent == "root")
    parentObj = svg->getRoot();
  else {
    CSVGObject::ObjectArray objects;

    svg->getRoot()->getHierChildrenOfId(parent.toStdString(), objects);

    if (objects.size() == 1)
      parentObj = objects[0];
  }

  //---

  QStringList names;

  if (parentObj) {
    CSVGObject::ObjectArray children;

    if (hier)
      parentObj->getAllChildren(children);
    else {
      for (const auto &child : parentObj->children())
        children.push_back(child);
    }

    for (const auto &child : children) {
      if (tag != "" && child->getTagName() != tag)
        continue;

      auto id = child->getHierId(/*autoName*/true);

      names.push_back(QString::fromStdString(id));
    }
  }

  mgr_->qtcl()->setResult(names);

  return true;
}

//---

void
GetSVGDataTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id"  , ArgType::String , "tag id");
  addArg(argv, "-name", ArgType::String , "value name");
  addArg(argv, "-hier", ArgType::Boolean, "hierarchical");
}

QStringList
GetSVGDataTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
GetSVGDataTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id   = argv.getParseStr ("id");
  auto name = argv.getParseStr ("name");
  auto hier = argv.getParseBool("hier");

  auto *svg = console_->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);
  else
    objects.push_back(svg->getRoot());

  //---

  if (name == "?") {
    auto names = QStringList() <<
     "@id" << "@parent" << "@depth" << "@tag_name" << "@selected" << "@viewBox" <<
     "@style" << "@text" << "@string";
    mgr_->qtcl()->setResult(names);
    return true;
  }

  //---

  QVariantList values;

  for (const auto &obj : objects) {
    if      (name == "@id") {
      values.push_back(QString::fromStdString(obj->getId()));
    }
    else if (name == "@parent") {
      auto *parent = obj->getParent();

      std::string id;

      if (parent)
        id = parent->getHierId(/*autoName*/true);

      values.push_back(QString::fromStdString(id));
    }
    else if (name == "@depth") {
      values.push_back(obj->getDepth());
    }
    else if (name == "@tag_name") {
      values.push_back(QString::fromStdString(obj->getTagName()));
    }
    else if (name == "@selected") {
      values.push_back(obj->getSelected());
    }
    else if (name == "@viewBox") {
      values.push_back(QString::fromStdString(obj->getViewBox().toString()));
    }
    else if (name == "@style") {
      values.push_back(QString::fromStdString(obj->styleString(/*flat*/true)));
    }
    else if (name == "@text") {
      values.push_back(QString::fromStdString(obj->getText()));
    }
    else if (name == "@string") {
      values.push_back(QString::fromStdString(obj->toString(hier)));
    }
    else {
      auto value = obj->getNameValue(name.toStdString());

      if (value.isValid())
        values.push_back(QString::fromStdString(value.getValue()));
    }
  }

  mgr_->qtcl()->setResult(values);

  return true;
}

//---

void
SetSVGDataTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id"   , ArgType::String , "tag id");
  addArg(argv, "-name" , ArgType::String , "value name");
  addArg(argv, "-value", ArgType::String , "new value");
  addArg(argv, "-hier" , ArgType::Boolean, "hierarchical");
}

QStringList
SetSVGDataTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
SetSVGDataTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id    = argv.getParseStr ("id");
  auto name  = argv.getParseStr ("name");
  auto value = argv.getParseStr ("value");
//auto hier  = argv.getParseBool("hier");

  auto *svg = console_->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);
  else
    objects.push_back(svg->getRoot());

  QVariantList values;

  for (const auto &obj : objects) {
    if      (name == "@text") {
      obj->setText(value.toStdString());
    }
    else if (name.left(6) == "style:")
      obj->setStyleValue(name.mid(6).toStdString(), value.toStdString());
    else
      obj->handleOption(name.toStdString(), value.toStdString());
  }

  mgr_->qtcl()->setResult(values);

  console_->window()->redraw(/*update*/true);

  return true;
}

//---

void
AddSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-parent", ArgType::String, "parent id");
  addArg(argv, "-tag"   , ArgType::String, "tag name");
  addArg(argv, "-id"    , ArgType::String, "new object id");
}

QStringList
AddSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
AddSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto parent = argv.getParseStr("parent");
  auto tag    = argv.getParseStr("tag");
  auto id     = argv.getParseStr("id");

  auto *svg = console_->window()->svg();

  CSVGObject *parentObj = nullptr;

  if (parent == "" || parent == "root")
    parentObj = svg->getRoot();
  else {
    CSVGObject::ObjectArray objects;

    svg->getRoot()->getHierChildrenOfId(parent.toStdString(), objects);

    if (objects.size() == 1)
      parentObj = objects[0];
    else
      return false;
  }

  auto *child = svg->createObjectByName(tag.toStdString());
  if (! child) return false;

  if (id != "")
    child->setId(id.toStdString());

  parentObj->addChildObject(child);

  auto id1 = child->getHierId(/*autoName*/true);

  mgr_->qtcl()->setResult(QString::fromStdString(id1));

  console_->window()->redraw(/*update*/true);

  return true;
}

//---

void
RemoveSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id", ArgType::String, "object id");
}

QStringList
RemoveSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
RemoveSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id = argv.getParseStr("id");

  auto *svg = console_->window()->svg();

  CSVGObject::ObjectArray objects;

  svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);

  for (auto *obj : objects)
    obj->ungroupObject();

  console_->window()->redraw(/*update*/true);

  return true;
}

//---

void
GetSVGSelectionTclCmd::
addArgs(CQTclCmd::CmdArgs &)
{
}

QStringList
GetSVGSelectionTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
GetSVGSelectionTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto *svg = console_->window()->svg();

  CSVG::ObjectList objects;

  svg->getSelectedObjects(objects);

  QStringList names;

  for (const auto &obj : objects) {
    auto id = obj->getHierId(/*autoName*/true);

    names.push_back(QString::fromStdString(id));
  }

  mgr_->qtcl()->setResult(names);

  return true;
}

//---

void
MoveSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id", ArgType::String, "object id");
  addArg(argv, "-to", ArgType::String, "new position");
  addArg(argv, "-by", ArgType::String, "delta position");
}

QStringList
MoveSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
MoveSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id = argv.getParseStr("id");
  auto to = argv.getParseStr("to");
  auto by = argv.getParseStr("by");

  auto *svg = console_->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);

  if (objects.empty()) {
    std::cerr << "Invalid object '" << id.toStdString() << "'\n";
    return false;
  }

  bool ok;

  if      (to != "") {
    QStringList strs;

    CQTclUtil::splitList(to, strs);

    if (strs.size() == 2) {
      double x = strs[0].toDouble(&ok);
      double y = strs[1].toDouble(&ok);

      for (const auto &obj : objects)
        obj->moveTo(CPoint2D(x, y));
    }
    else {
      std::cerr << "Invalid point '" << to.toStdString() << "'\n";
      return false;
    }
  }
  else if (by != "") {
    QStringList strs;

    CQTclUtil::splitList(by, strs);

    if (strs.size() == 2) {
      double x = strs[0].toDouble(&ok);
      double y = strs[1].toDouble(&ok);

      for (const auto &obj : objects)
        obj->moveBy(CVector2D(x, y));
    }
    else {
      std::cerr << "Invalid point '" << by.toStdString() << "'\n";
      return false;
    }
  }

  console_->window()->redraw(/*update*/true);

  return true;
}

//---

void
ScaleSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id", ArgType::String, "object id");
  addArg(argv, "-by", ArgType::String, "scale factor");
}

QStringList
ScaleSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
ScaleSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id = argv.getParseStr("id");
  auto by = argv.getParseStr("by");

  auto *svg = console_->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);

  if (objects.empty()) {
    std::cerr << "Invalid object '" << id.toStdString() << "'\n";
    return false;
  }

  bool ok;

  if (by != "") {
    double s = by.toDouble(&ok);

    for (const auto &obj : objects)
      obj->scaleBy(s);
  }

  console_->window()->redraw(/*update*/true);

  return true;
}

//---

void
RotateSVGObjectTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-id", ArgType::String, "object id");
  addArg(argv, "-by", ArgType::String, "delta angle");
  addArg(argv, "-at", ArgType::String, "rotation center");
}

QStringList
RotateSVGObjectTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
RotateSVGObjectTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto id = argv.getParseStr("id");
  auto by = argv.getParseStr("by");
  auto at = argv.getParseStr("at");

  auto *svg = console_->window()->svg();

  CSVGObject::ObjectArray objects;

  if (id != "")
    svg->getRoot()->getHierChildrenOfId(id.toStdString(), objects);

  if (objects.empty()) {
    std::cerr << "Invalid object '" << id.toStdString() << "'\n";
    return false;
  }

  bool ok;

  if (by != "") {
    double a = M_PI*by.toDouble(&ok)/180.0;

     if      (at != "") {
      QStringList strs;

      CQTclUtil::splitList(at, strs);

      if (strs.size() == 2) {
        double x = strs[0].toDouble(&ok);
        double y = strs[1].toDouble(&ok);

        for (const auto &obj : objects)
          obj->rotateAt(a, CPoint2D(x, y));
      }
      else {
        std::cerr << "Invalid point '" << at.toStdString() << "'\n";
        return false;
      }
    }
    else {
      for (const auto &obj : objects)
        obj->rotateBy(a);
    }
  }

  console_->window()->redraw(/*update*/true);

  return true;
}

//---
//---

void
WriteSVGFileTclCmd::
addArgs(CQTclCmd::CmdArgs &argv)
{
  addArg(argv, "-file", ArgType::String, "filename");
}

QStringList
WriteSVGFileTclCmd::
getArgValues(const QString &, const NameValueMap &)
{
  return QStringList();
}

bool
WriteSVGFileTclCmd::
exec(CQTclCmd::CmdArgs &argv)
{
  addArgs(argv);

  bool rc;

  if (! argv.parse(rc))
    return rc;

  auto file = argv.getParseStr("file");

  auto *svg = console_->window()->svg();

  std::ofstream os(file.toStdString());

  svg->print(os, /*hier*/true);

  return true;
}

//---

}
