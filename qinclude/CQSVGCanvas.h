#ifndef CQSVGCanvas_H
#define CQSVGCanvas_H

#include <CBBox2D.h>
#include <QFrame>

class CQSVG;
class CQSVGWindow;
class CQSVGRenderer;
class CSVGObject;

class CQSVGCanvas : public QFrame {
  Q_OBJECT

 public:
  enum class Shape {
    CROSS,
    PLUS,
    CIRCLE
  };

 public:
  CQSVGCanvas(CQSVGWindow *window);
 ~CQSVGCanvas();

  //---

  CQSVGWindow *window() const { return window_; }

  CQSVG *svg() const { return svg_; }

  CQSVGRenderer *renderer() const { return renderer_; }

  //---

  const CPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &o) { offset_ = o; }

  double scale() const { return scale_; }
  void setScale(double r) { scale_ = r; }

  void resizeEvent(QResizeEvent *) override;

  void paintEvent(QPaintEvent *) override;

  void mousePressEvent  (QMouseEvent *e) override;
  void mouseMoveEvent   (QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;

  void keyPressEvent(QKeyEvent *ke) override;

  bool event(QEvent *e) override;

  void drawSelected();

  void drawRect(const CBBox2D &bbox, const QColor &c);
  void drawLine(const CPoint2D &p1, const CPoint2D &p2, const QColor &c);

  void drawPoints(std::initializer_list<CPoint2D> points, Shape shape, int size,
                  const QColor &bg, const QColor &fg);

  void drawPoint(const CPoint2D &p, Shape shape, int size, const QColor &bg, const QColor &fg);

  CPoint2D pixelToWindow(const CPoint2D &p) const;

 public slots:
  void redraw();

 private:
  CQSVGWindow*   window_    { nullptr };
  CQSVG*         svg_       { nullptr };
  CQSVGRenderer* renderer_  { nullptr };
  QPainter*      opainter_  { nullptr };
  QImage         oimage_;
  CPoint2D       offset_    { 0, 0 };
  double         scale_     { 1 };
  bool           autoScale_ { false };
};

#endif
