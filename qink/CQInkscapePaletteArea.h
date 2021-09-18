#ifndef CQInkscapePaletteArea_H
#define CQInkscapePaletteArea_H

#include <QFrame>

class QVBoxLayout;

namespace CQInkscape {

class Window;
class Palette;
class PaletteHandle;

class PaletteArea : public QFrame {
  Q_OBJECT

 public:
  PaletteArea(Window *window);

  Window *window() const { return window_; }

  void addPalette(Palette *palette);

  void showPalettes();

  bool moveHandle(int dx);

  QSize sizeHint() const override;

 protected:
  using Palettes = std::vector<Palette *>;

  Window*        window_ { nullptr };
  PaletteHandle* handle_ { nullptr };
  QVBoxLayout*   layout_ { nullptr };
  Palettes       palettes_;
  int            width_  { -1 };
};

//---

class PaletteHandle : public QFrame {
  Q_OBJECT

 public:
  PaletteHandle(PaletteArea *area);

  PaletteArea *area() { return area_; }

 private:
  friend class PaletteArea;

  void updateState();

  void mousePressEvent  (QMouseEvent *e) override;
  void mouseMoveEvent   (QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;

  void enterEvent(QEvent *e) override;
  void leaveEvent(QEvent *e) override;

  void paintEvent(QPaintEvent *) override;

 private:
  struct MouseState {
    bool   pressed;
    QPoint pressPos;

    MouseState() {
      pressed = false;
    }
  };

  PaletteArea *area_      { nullptr };
  MouseState   mouseState_;
  bool         mouseOver_ { false };
};

}

#endif
