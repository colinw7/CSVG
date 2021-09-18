#ifndef CQInkscapeMouseToolBar_H
#define CQInkscapeMouseToolBar_H

#include <QFrame>

class QStackedWidget;

namespace CQInkscape {

class Window;

class MouseToolBar : public QFrame {
  Q_OBJECT

 public:
  MouseToolBar(Window *window);

  void updateState();

 private slots:
  void zoomInSlot();
  void zoomOutSlot();
  void zoomFitSlot();

 private:
  Window* window_ { nullptr };

  QStackedWidget *stack_ { nullptr };

  QFrame *selectFrame_      { nullptr };
  QFrame *pointSelectFrame_ { nullptr };
  QFrame *zoomFrame_        { nullptr };
  QFrame *rectFrame_        { nullptr };
  QFrame *pathFrame_        { nullptr };
  QFrame *textFrame_        { nullptr };
};

}

#endif
