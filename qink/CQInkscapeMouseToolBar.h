#ifndef CQInkscapeMouseToolBar_H
#define CQInkscapeMouseToolBar_H

#include <QFrame>

class CQRealSpin;
class QStackedWidget;

namespace CQInkscape {

class Window;

class MouseToolBar : public QFrame {
  Q_OBJECT

 public:
  MouseToolBar(Window *window);

  Window *window() const { return window_; }

  void updateState();

 private slots:
  void zoomInSlot();
  void zoomOutSlot();
  void zoomFitSlot();

  void rectWidthSlot();
  void rectHeightSlot();
  void rectXRadiusSlot();
  void rectYRadiusSlot();

  void ellipseXRadiusSlot();
  void ellipseYRadiusSlot();
  void ellipseStartAngleSlot();
  void ellipseEndAngleSlot();

 private:
  Window* window_ { nullptr };

  QStackedWidget *stack_ { nullptr };

  struct RectData {
    QFrame*     frame  { nullptr };
    CQRealSpin* wEdit  { nullptr };
    CQRealSpin* hEdit  { nullptr };
    CQRealSpin* rxEdit { nullptr };
    CQRealSpin* ryEdit { nullptr };
  };

  struct EllipseData {
    QFrame*     frame     { nullptr };
    CQRealSpin* rxEdit    { nullptr };
    CQRealSpin* ryEdit    { nullptr };
    CQRealSpin* startEdit { nullptr };
    CQRealSpin* endEdit   { nullptr };
  };

  QFrame*     selectFrame_      { nullptr };
  QFrame*     pointSelectFrame_ { nullptr };
  QFrame*     zoomFrame_        { nullptr };
  RectData    rectData_;
  EllipseData ellipseData_;
  QFrame*     pathFrame_        { nullptr };
  QFrame*     textFrame_        { nullptr };
};

}

#endif
