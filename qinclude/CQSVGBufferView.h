#ifndef CQSVGBufferCanvas_H
#define CQSVGBufferCanvas_H

#include <CBBox2D.h>
#include <QFrame>

class CQSVGBufferCanvas;
class CSVG;

class QComboBox;
class QCheckBox;
class QLabel;

class CQSVGBufferView : public QFrame {
  Q_OBJECT

 public:
  CQSVGBufferView(CSVG *svg);

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
  CSVG*              svg_         { nullptr };
  QComboBox*         combo_       { nullptr };
  QCheckBox*         bgCheck_     { nullptr };
  CQSVGBufferCanvas* canvas_      { nullptr };
  QLabel*            statusLabel_ { nullptr };
  QLabel*            posLabel_    { nullptr };
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
  CQSVGBufferView *view_ { nullptr };
};

#endif
