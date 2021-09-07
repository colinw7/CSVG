#ifndef CQInkscapeStatusBar_H
#define CQInkscapeStatusBar_H

#include <QFrame>

#include <CPoint2D.h>

class QLabel;

namespace CQInkscape {

class Window;
class FillStrokeStatus;
class PosLabel;

class StatusBar : public QFrame {
  Q_OBJECT

 public:
  StatusBar(Window *window);

  void updateCurrent();

  void setMousePos(const CPoint2D &p);

 private:
  Window*           window_     { nullptr };
  FillStrokeStatus* fillStroke_ { nullptr };
  QLabel*           msgLabel_   { nullptr };
  PosLabel*         posLabel_   { nullptr };
};

//---

class FillStrokeStatus : public QFrame {
  Q_OBJECT

 public:
  FillStrokeStatus(Window *window);

  void paintEvent(QPaintEvent *) override;

  QSize sizeHint() const override;

 private:
  Window*  window_ { nullptr };
  CPoint2D p_;
  int      width_  { 0 };
};

//---

class PosLabel : public QFrame {
  Q_OBJECT

 public:
  PosLabel(Window *window);

  void setMessage(const QString &msg);

  void setPos(const CPoint2D &p);

  void paintEvent(QPaintEvent *) override;

 private:
  Window*  window_ { nullptr };
  CPoint2D p_;
};

}

#endif
