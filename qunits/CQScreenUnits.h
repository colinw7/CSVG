#include <QWidget>

class QLineEdit;
class QLabel;

class Window : public QWidget {
  Q_OBJECT

 public:
  Window();

  void setLabel(const QString &s);

 private slots:
  void dpiSlot();

 private:
  QLineEdit *dpiEdit_ { 0 };
  QLabel    *label_ { 0 };
};

class Canvas : public QWidget {
  Q_OBJECT

 public:
  Canvas(Window *window);

  void paintEvent(QPaintEvent *e);

  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);

  void keyPressEvent(QKeyEvent *e);

  QSize sizeHint() const;

  void setInfo();

 private:
  Window *window_  { 0 };
  bool    pressed_ { false };
  double  pressX_  { 0 };
};
