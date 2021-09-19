#ifndef CQInkscapeConsole_H
#define CQInkscapeConsole_H

#include <CQTclCommand.h>
#include <QFrame>

namespace CQInkscape {

class Window;
class ConsoleCommandWidget;
class ConsoleScrollArea;

class Console : public QFrame {
  Q_OBJECT

 public:
  Console(Window *window);
 ~Console();

  Window *window() const { return window_; }

  ConsoleScrollArea *area() const { return area_; }

  QSize sizeHint() const override;

  void loadTclFile(const QString &file);

 private slots:
  void executeCommand(const QString &str);

 private:
  Window* window_ { nullptr };

  ConsoleScrollArea *area_ { nullptr };
};

}

//---

namespace CQInkscape {

class ConsoleCommandWidget : public CQTclCommand::CommandWidget {
 public:
  ConsoleCommandWidget(ConsoleScrollArea *);
 ~ConsoleCommandWidget();

  void addCommand(const QString &name, CQTclCmd::CmdProc *proc);

  void executeCommand(const QString &line);

  int eval(const QString &line, bool showError=true, bool showResult=false);

  bool complete(const QString &text, int pos, QString &newText,
                CompleteMode completeMode) const override;
};

//---

class ConsoleScrollArea : public CQTclCommand::ScrollArea {
 public:
  ConsoleScrollArea(Console *console);

  Console *console() const { return console_; }

  CQCommand::CommandWidget *createCommandWidget() const override;

  void executeCommand(const QString &line);

  int eval(const QString &line, bool showError=true, bool showResult=false);

  void addCommand(const QString &name, CQTclCmd::CmdProc *proc);

 private:
  Console* console_ { nullptr };
};

}

//---

#define CQINKSCAPE_TCL_CMD(NAME) \
CQTCL_CMD(NAME, TclCmdProc, ConsoleScrollArea)

#define CQINKSCAPE_INST_TCL_CMD(NAME) \
CQTCL_INST_TCL_CMD(NAME, TclCmdProc, ConsoleScrollArea)

//---

namespace CQInkscape {

class TclCmdProc : public CQTclCommand::TclCmdProc {
 public:
  TclCmdProc(ConsoleScrollArea *area);
};

CQINKSCAPE_TCL_CMD(Help)

CQINKSCAPE_TCL_CMD(GetSVGChildren)
CQINKSCAPE_TCL_CMD(GetSVGData)
CQINKSCAPE_TCL_CMD(SetSVGData)
CQINKSCAPE_TCL_CMD(AddSVGObject)
CQINKSCAPE_TCL_CMD(RemoveSVGObject)
CQINKSCAPE_TCL_CMD(GetSVGSelection)
CQINKSCAPE_TCL_CMD(MoveSVGObject)
CQINKSCAPE_TCL_CMD(ScaleSVGObject)
CQINKSCAPE_TCL_CMD(RotateSVGObject)
CQINKSCAPE_TCL_CMD(WriteSVGFile)

}

#endif
