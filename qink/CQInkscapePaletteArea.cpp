#include <CQInkscapePaletteArea.h>
#include <CQInkscapeWindow.h>
#include <CQInkscapePalette.h>

#include <CQUtil.h>

#include <QSplitterHandle>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStylePainter>

namespace CQInkscape {

PaletteArea::
PaletteArea(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("paletteArea");

  auto *slayout = CQUtil::makeLayout<QHBoxLayout>(this, 0, 0);

  handle_ = new PaletteHandle(this);

  slayout->addWidget(handle_);

  layout_ = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  slayout->addLayout(layout_);
}

void
PaletteArea::
addPalette(Palette *palette)
{
  palettes_.push_back(palette);

  layout_->addWidget(palette);
}

void
PaletteArea::
showPalettes()
{
  for (const auto &palette : palettes_)
    palette->show();
}

bool
PaletteArea::
moveHandle(int dx)
{
  if (dx == 0)
    return false;

  if (dx < 0 && dx + this->width() < 16)
    return false;

  width_ = this->width() - dx;

  window_->updatePlacement();

  return true;
}

QSize
PaletteArea::
sizeHint() const
{
  int width = width_;

  if (width < 0) {
    // get width of palettes (TODO: palette sides)
    width = 0;

    for (const auto &palette : palettes_)
      width += palette->sizeHint().width();

    width += handle_->width();
  }

  return QSize(width, 0);
}

//---

PaletteHandle::
PaletteHandle(PaletteArea *area) :
 area_(area)
{
  setObjectName("handle");

  setFixedWidth(4);

  setCursor(Qt::SplitHCursor);

  setMouseTracking(true);
}

void
PaletteHandle::
paintEvent(QPaintEvent *)
{
  QStylePainter ps(this);

  QStyleOption opt;

  opt.initFrom(this);

  opt.rect  = rect();
  opt.state = QStyle::State_Horizontal;

  if (mouseState_.pressed)
    opt.state |= QStyle::State_Sunken;

  if (mouseOver_)
    opt.state |= QStyle::State_MouseOver;

  ps.drawControl(QStyle::CE_Splitter, opt);
}

void
PaletteHandle::
mousePressEvent(QMouseEvent *e)
{
  mouseState_.pressed  = true;
  mouseState_.pressPos = e->globalPos();

  update();
}

void
PaletteHandle::
mouseMoveEvent(QMouseEvent *e)
{
  if (! mouseState_.pressed) return;

  int dx = e->globalPos().x() - mouseState_.pressPos.x();

  if (dx) {
    if (area()->moveHandle(dx))
      mouseState_.pressPos = e->globalPos();
  }

  update();
}

void
PaletteHandle::
mouseReleaseEvent(QMouseEvent *)
{
  mouseState_.pressed = false;

  update();
}

void
PaletteHandle::
enterEvent(QEvent *)
{
  mouseOver_ = true;

  update();
}

void
PaletteHandle::
leaveEvent(QEvent *)
{
  mouseOver_ = false;

  update();
}

}
