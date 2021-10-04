#ifndef CQInkscapePaletteArea_H
#define CQInkscapePaletteArea_H

#include <QFrame>

class QVBoxLayout;

namespace CQInkscape {

class Window;
class Palette;
class PaletteHandle;
class PaletteFrame;

class PaletteArea : public QFrame {
  Q_OBJECT

  Q_PROPERTY(LayoutType layoutType READ layoutType WRITE setLayoutType)

  Q_ENUMS(LayoutType)

 public:
  enum class LayoutType {
    SINGLE,
    ALL
  };

 public:
  PaletteArea(Window *window);

  Window *window() const { return window_; }

  const LayoutType &layoutType() const { return layoutType_; }
  void setLayoutType(const LayoutType &t);

  int headerHeight() const { return headerHeight_; }

  void addPalette(Palette *palette);

  void showPalettes();

  int maxPaletteWidth() const;

  bool moveHandle(int dx);

  void resizeEvent(QResizeEvent *) override;

  void paintEvent(QPaintEvent *) override;

  QSize sizeHint() const override;

 protected:
  friend class PaletteFrame;

  void updateLayout();

  void drawHeaders(QPainter *painter);

  void headerMove (const QPoint &p);
  void headerPress(const QPoint &p);

 protected:
  using Palettes = std::vector<Palette *>;

  struct HeaderData {
    QRect hrect;
    QRect rect;
  };

  using HeaderDatas = std::vector<HeaderData>;

  Window*        window_         { nullptr };
  PaletteHandle* handle_         { nullptr };
  PaletteFrame*  frame_          { nullptr };
  Palettes       palettes_;
  int            currentPalette_ { -1 };
  int            insidePalette_  { -1 };
  HeaderDatas    headerDatas_;
  int            headerHeight_   { 32 };
  int            width_          { -1 };
  LayoutType     layoutType_     { LayoutType::SINGLE };
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

//---

class PaletteFrame : public QFrame {
  Q_OBJECT

 public:
  PaletteFrame(PaletteArea *area);

  PaletteArea *area() { return area_; }

  QSize sizeHint() const override;

 private:
  friend class PaletteArea;

  void contextMenuEvent(QContextMenuEvent *e) override;

  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *) override;

  void enterEvent(QEvent *) override;
  void leaveEvent(QEvent *) override;

  void resizeEvent(QResizeEvent *) override;

  void paintEvent(QPaintEvent *) override;

 private slots:
  void layoutAllSlot();
  void layoutOneSlot();

 private:
  PaletteArea *area_ { nullptr };
};

}

#endif
