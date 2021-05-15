#ifndef CQSVGBufferCanvas_H
#define CQSVGBufferCanvas_H

#include <CBBox2D.h>
#include <QFrame>

class CQSVG;
class CQSVGBufferCanvas;
class QComboBox;
class QCheckBox;
class QLabel;

class CQSVGBufferView : public QFrame {
  Q_OBJECT

 public:
  CQSVGBufferView(CQSVG *qsvg);

  void updateState();

  QString bufferName() const;

  QImage bufferImage() const;

  CPoint2D bufferOrigin() const;

  CBBox2D bufferBBox() const;

  void showPos(const QPoint &ppos);

  bool isCheckerboard() const;

 private slots:
  void updateBuffer();

 private:
  CQSVG*             qsvg_        { 0 };
  QComboBox*         combo_       { 0 };
  QCheckBox*         bgCheck_     { 0 };
  CQSVGBufferCanvas* canvas_      { 0 };
  QLabel*            statusLabel_ { 0 };
  QLabel*            posLabel_    { 0 };
};

//---

class CQSVGBufferCanvas : public QWidget {
  Q_OBJECT

 public:
  CQSVGBufferCanvas(CQSVGBufferView *view);

  void paintEvent(QPaintEvent *) override;

  void mouseMoveEvent(QMouseEvent *e) override;

  QSize sizeHint() const override;

 private:
  CQSVGBufferView *view_ { 0 };
};

#endif
