#ifndef CQSVGCanvas_H
#define CQSVGCanvas_H

#include <CBBox2D.h>
#include <QWidget>

class CQSVG;
class CQSVGWindow;
class CQSVGRenderer;
class CSVGObject;

class CQSVGCanvas : public QWidget {
  Q_OBJECT

 public:
  CQSVGCanvas(CQSVGWindow *window, CQSVG *svg);

  CQSVGRenderer *renderer() const { return renderer_; }

  double scale() const { return scale_; }
  void setScale(double r) { scale_ = r; }

  void resizeEvent(QResizeEvent *);

  void paintEvent(QPaintEvent *);

  void mousePressEvent  (QMouseEvent *e);
  void mouseMoveEvent   (QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

  void keyPressEvent(QKeyEvent *ke);

  void drawSelected();

  void drawRect(const CBBox2D &bbox, const QColor &c);

 public slots:
  void redraw();

 private:
  CQSVGWindow   *window_   { 0 };
  CQSVG         *svg_      { 0 };
  CQSVGRenderer *renderer_ { 0 };
  QPainter      *opainter_ { 0 };
  QImage         oimage_;
  double         scale_    { 1 };
};

#endif
