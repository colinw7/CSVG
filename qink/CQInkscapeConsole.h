#ifndef CQInkscapeConsole_H
#define CQInkscapeConsole_H

#include <QFrame>

namespace CQCommand {
class ScrollArea;
}

class CQTcl;

namespace CQTclCmd {
class Mgr;
}

namespace CQInkscape {

class Window;
class ConsoleCommandWidget;

class Console : public QFrame {
  Q_OBJECT

 public:
  Console(Window *window);
 ~Console();

  Window *window() const { return window_; }

  CQTclCmd::Mgr *tclCmdMgr() const { return mgr_; }

  QSize sizeHint() const override;

  bool complete(ConsoleCommandWidget *widget, const QString &text, int pos,
                QString &newText, bool interactive) const;

  void loadTclFile(const QString &file);

 private slots:
  void executeCommand(const QString &str);

 private:
  Window* window_ { nullptr };

  CQTcl*         qtcl_ { nullptr };
  CQTclCmd::Mgr* mgr_  { nullptr };

  CQCommand::ScrollArea *command_ { nullptr };
};

}

//---

#include <CQInkscapeTclCmd.h>

namespace CQInkscape {

CQINKSCAPE_TCL_CMD(Help)

CQINKSCAPE_TCL_CMD(GetSVGObjects)
CQINKSCAPE_TCL_CMD(GetSVGData)
CQINKSCAPE_TCL_CMD(SetSVGData)
CQINKSCAPE_TCL_CMD(AddSVGChild)
CQINKSCAPE_TCL_CMD(WriteSVG)

}

#endif
