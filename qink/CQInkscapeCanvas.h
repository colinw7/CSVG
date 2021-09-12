#ifndef CQInkscapeCanvas_H
#define CQInkscapeCanvas_H

#include <QFrame>
#include <CDisplayRange2D.h>
#include <CMatrix2D.h>

class CQSVGRenderer;
class CSVG;

namespace CQInkscape {

class Window;

class Canvas : public QFrame {
  Q_OBJECT

 public:
  Canvas(Window *window);

  CSVG *svg() const { return svg_; }

  Window *window() const { return window_; }

  void redraw(bool update);

  void zoomIn();
  void zoomOut();
  void zoomReset();

  void resizeEvent(QResizeEvent *) override;

  void paintEvent(QPaintEvent *) override;

  void mousePressEvent(QMouseEvent *me) override;
  void mouseMoveEvent (QMouseEvent *me) override;

  void keyPressEvent(QKeyEvent *ke) override;

  bool event(QEvent *e) override;

  CPoint2D pixelToWindow(const CPoint2D &p) const;
  CPoint2D windowToPixel(const CPoint2D &p) const;

 private:
  void updateMatrix();

 private:
  Window*        window_   { nullptr };
  CSVG*          svg_      { nullptr };
  CQSVGRenderer* renderer_ { nullptr };
  QPainter*      opainter_ { nullptr };
  CPoint2D       offset_   { 0.0, 0.0 };
  double         scale_    { 1.0 };

  mutable CMatrix2D       matrix_;
  mutable CMatrix2D       imatrix_;
  mutable CDisplayRange2D range_;
};

}

#endif
