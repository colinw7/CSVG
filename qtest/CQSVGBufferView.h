#ifndef CQSVGBufferCanvas_H
#define CQSVGBufferCanvas_H

#include <CImagePtr.h>
#include <CBBox2D.h>
#include <QFrame>

class CQSVG;
class CQSVGBufferCanvas;
class QComboBox;
class QLabel;

class CQSVGBufferView : public QFrame {
 public:
  CQSVGBufferView(CQSVG *qsvg);

  void updateState();

  QString bufferName() const;

  //CImagePtr bufferImage() const;
  QImage bufferImage() const;

  CPoint2D bufferOrigin() const;

  CBBox2D bufferBBox() const;

  void showPos(const QPoint &ppos);

 private:
  CQSVG*             qsvg_     { 0 };
  QComboBox*         combo_    { 0 };
  CQSVGBufferCanvas* canvas_   { 0 };
  QLabel*            posLabel_ { 0 };
};

//---

class CQSVGBufferCanvas : public QWidget {
 public:
  CQSVGBufferCanvas(CQSVGBufferView *view);

  void paintEvent(QPaintEvent *);

  void mouseMoveEvent(QMouseEvent *e);

  QSize sizeHint() const;

 private:
  CQSVGBufferView *view_    { 0 };
  bool             checked_ { true };
};

#endif
