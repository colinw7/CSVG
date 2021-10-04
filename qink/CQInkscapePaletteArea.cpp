#include <CQInkscapePaletteArea.h>
#include <CQInkscapeWindow.h>
#include <CQInkscapePalette.h>

#include <CQUtil.h>

#include <QSplitterHandle>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QStylePainter>
#include <QMenu>
#include <QContextMenuEvent>

namespace CQInkscape {

PaletteArea::
PaletteArea(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("paletteArea");

  auto *layout = CQUtil::makeLayout<QHBoxLayout>(this, 0, 0);

  handle_ = new PaletteHandle(this);
  frame_  = new PaletteFrame (this);

  layout->addWidget(handle_);
  layout->addWidget(frame_);
}

void
PaletteArea::
setLayoutType(const LayoutType &t)
{
  layoutType_ = t;

  updateLayout();

  update();
}

void
PaletteArea::
addPalette(Palette *palette)
{
  palettes_.push_back(palette);

  palette->setParent(frame_);

  if (currentPalette_ < 0)
    currentPalette_ = 0;

  headerDatas_.resize(palettes_.size());
}

void
PaletteArea::
updateLayout()
{
  QFontMetrics fm(font());

  headerHeight_ = fm.height() + 4;

  //---

  if      (layoutType() == LayoutType::ALL) {
    for (const auto &palette : palettes_)
      palette->setVisible(true);

    int y = 0;
    int w = frame_->width();

    int i = 0;

    for (const auto &palette : palettes_) {
      auto size = palette->sizeHint();

      QRect r(QPoint(0, y), QSize(w, size.height()));

      palette->setGeometry(r);

      headerDatas_[i].rect = r;

      y += r.height();

      ++i;
    }
  }
  else if (layoutType() == LayoutType::SINGLE) {
    int i = 0;

    for (const auto &palette : palettes_) {
      palette->setVisible(false);

      headerDatas_[i].rect = QRect();

      ++i;
    }

    int y = (currentPalette_ >= 0 ? headerHeight()*(currentPalette_ + 1) : 0);
    int w = frame_->width();
    int h = frame_->height() - palettes_.size()*headerHeight();

    if (currentPalette_ >= 0) {
      auto *palette = palettes_[currentPalette_];

      palette->setVisible(true);

      QRect r(QPoint(0, y), QSize(w, h));

      palette->setGeometry(r);

      headerDatas_[currentPalette_].rect = r;
    }
  }
}

int
PaletteArea::
maxPaletteWidth() const
{
  int w = 0;

  if      (layoutType() == LayoutType::ALL) {
    for (const auto &palette : palettes_) {
      auto size = palette->sizeHint();

      w = std::max(w, size.width());
    }
  }
  else if (layoutType() == LayoutType::SINGLE) {
    if (currentPalette_ >= 0) {
      auto *palette = palettes_[currentPalette_];

      auto size = palette->sizeHint();

      w = std::max(w, size.width());
    }
  }

  return w;
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

void
PaletteArea::
resizeEvent(QResizeEvent *)
{
  updateLayout();
}

void
PaletteArea::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.fillRect(this->rect(), palette().color(QPalette::Window));
}

void
PaletteArea::
drawHeaders(QPainter *painter)
{
  if (layoutType() == LayoutType::ALL) {
    for (auto &headerData : headerDatas_)
      headerData.hrect = QRect();

    return;
  }

  //---

  QFontMetrics fm(font());

  int fa = fm.ascent ();
  int fd = fm.descent();

  int i = 0;
  int y = 0;

  for (const auto &palette : palettes_) {
    QRect r(0, y, width(), headerHeight());

    QColor bg;

    if      (i == insidePalette_)
      bg = QColor(255, 255, 255);
    else if (i == currentPalette_)
      bg = QColor(240, 240, 240);
    else
      bg = QColor(240, 240, 255);

    painter->fillRect(r, bg);

    int xt = 2;
    int yt = y + headerHeight()/2.0 + (fa - fd)/2;

    painter->drawText(xt, yt, palette->name());

    if (i == currentPalette_)
      y += palette->height();

    y += headerHeight();

    headerDatas_[i].hrect = r;

    ++i;
  }
}

void
PaletteArea::
headerPress(const QPoint &p)
{
  bool changed = false;

  int i = 0;

  for (const auto &headerData : headerDatas_) {
    if (headerData.hrect.contains(p)) {
      currentPalette_ = i;
      changed = true;
      break;
    }

    ++i;
  }

  if (changed)
    updateLayout();
}

void
PaletteArea::
headerMove(const QPoint &p)
{
  int insidePalette = -1;

  int i = 0;

  for (const auto &headerData : headerDatas_) {
    if (headerData.hrect.contains(p)) {
      insidePalette = i;
      break;
    }

    ++i;
  }

  if (insidePalette != insidePalette_) {
    insidePalette_ = insidePalette;

    frame_->update();
  }
}

QSize
PaletteArea::
sizeHint() const
{
  int width = width_;

  if (width < 0) {
    // get width of palettes (TODO: palette sides)
    width = maxPaletteWidth();

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

  ps.fillRect(rect(), palette().color(QPalette::Dark));

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

//------

PaletteFrame::
PaletteFrame(PaletteArea *area) :
 area_(area)
{
  setObjectName("paletteArea");

  setContextMenuPolicy(Qt::DefaultContextMenu);

  setMouseTracking(true);
}

void
PaletteFrame::
contextMenuEvent(QContextMenuEvent *e)
{
  auto *menu = new QMenu;

  auto *allAction = menu->addAction("All");
  auto *oneAction = menu->addAction("One");

  connect(allAction, SIGNAL(triggered()), this, SLOT(layoutAllSlot()));
  connect(oneAction, SIGNAL(triggered()), this, SLOT(layoutOneSlot()));

  //---

  (void) menu->exec(e->globalPos());

  delete menu;
}

void
PaletteFrame::
layoutAllSlot()
{
  area_->setLayoutType(PaletteArea::LayoutType::ALL);
}

void
PaletteFrame::
layoutOneSlot()
{
  area_->setLayoutType(PaletteArea::LayoutType::SINGLE);
}

void
PaletteFrame::
mousePressEvent(QMouseEvent *e)
{
  area_->headerPress(e->pos());
}

void
PaletteFrame::
mouseMoveEvent(QMouseEvent *e)
{
  area_->headerMove(e->pos());
}

void
PaletteFrame::
enterEvent(QEvent *)
{
}

void
PaletteFrame::
leaveEvent(QEvent *)
{
  area_->headerMove(QPoint(-1, -1));
}

void
PaletteFrame::
resizeEvent(QResizeEvent *)
{
  area_->updateLayout();
}

void
PaletteFrame::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  area_->drawHeaders(&painter);
}

QSize
PaletteFrame::
sizeHint() const
{
  int w = area_->maxPaletteWidth();
  int h = area_->height();

  return QSize(w, h);
}

}
