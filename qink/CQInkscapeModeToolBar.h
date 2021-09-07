#ifndef CQInkscapeModeToolBar_H
#define CQInkscapeModeToolBar_H

#include <QFrame>

namespace CQInkscape {

class Window;

class ModeToolBar : public QFrame {
  Q_OBJECT

 public:
  ModeToolBar(Window *window);

 private slots:
  void selectSlot();
  void pointSelectSlot();
  void zoomSlot();
  void createRectSlot();
  void createPathSlot();
  void createTextSlot();

 private:
  Window* window_ { nullptr };
};

}

#endif
