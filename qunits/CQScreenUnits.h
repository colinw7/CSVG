#include <QWidget>

class QLineEdit;

class Window : public QWidget {
  Q_OBJECT

 public:
  Window();

 private slots:
  void dpiSlot();

 private:
  QLineEdit *dpiEdit_;
};

class Canvas : public QWidget {
  Q_OBJECT

 public:
  Canvas();

  void paintEvent(QPaintEvent *e);

  void mousePressEvent(QMouseEvent *e);

  void keyPressEvent(QKeyEvent *e);

  QSize sizeHint() const;
};
