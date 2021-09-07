#include <CQInkscapeColorBar.h>
#include <CQInkscapeWindow.h>

#include <CSVGObject.h>
#include <CQUtil.h>

#include <QPainter>
#include <QMenu>
#include <QContextMenuEvent>

namespace CQInkscape {

ColorBar::
ColorBar(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("colorBar");

  setContextMenuPolicy(Qt::DefaultContextMenu);

  addPalettes();
}

void
ColorBar::
addPalettes()
{
  ColorPalette lightPalette("Light Icon");

  lightPalette.addColor(QColor( 84,  63, 124));
  lightPalette.addColor(QColor(149, 116, 174));
  lightPalette.addColor(QColor(244, 129,  50));
  lightPalette.addColor(QColor(101, 153,  65));
  lightPalette.addColor(QColor(126, 126, 141));
  lightPalette.addColor(QColor(205, 191, 218));
  lightPalette.addColor(QColor(207,  89,  89));
  lightPalette.addColor(QColor(  2, 105, 165));
  lightPalette.addColor(QColor(247, 234, 110));
  lightPalette.addColor(QColor(255, 255, 255));
  lightPalette.addColor(QColor(  0,   0,   0));

  palettes_.push_back(lightPalette);
}

void
ColorBar::
mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton) {
    auto pos = e->pos();

    int ind = (swatchWidth_ > 0.0 ? pos.x()/swatchWidth_ : 0.0);
    //std::cerr << ind << "\n";

    setFill(ind);
  }
}

void
ColorBar::
setFill(int ind)
{
  auto *obj = window_->currentObject();

  if (obj) {
    auto fill = obj->getFill();

    auto c = (ind > 0 ? paletteColor(ind - 1) : Qt::transparent);

    fill.setColor(CSVGFill::Color(CSVGColor(CQUtil::colorToRGBA(c))));
    fill.setOpacity(CSVGFill::Opacity(c.alphaF()));

    obj->setFill(fill);

    window_->redraw(/*update*/true);
  }
}

void
ColorBar::
setStroke(int ind)
{
  auto *obj = window_->currentObject();

  if (obj) {
    auto stroke = obj->getStroke();

    auto c = (ind > 0 ? paletteColor(ind - 1) : Qt::transparent);

    stroke.setColor(CSVGStroke::Color(CSVGColor(CQUtil::colorToRGBA(c))));
    stroke.setOpacity(CSVGFill::Opacity(c.alphaF()));

    obj->setStroke(stroke);

    window_->redraw(/*update*/true);
  }
}

void
ColorBar::
contextMenuEvent(QContextMenuEvent *e)
{
  auto *menu = new QMenu;

  auto pos = e->pos();

  menuInd_ = (swatchWidth_ > 0.0 ? pos.x()/swatchWidth_ : 0.0);

  //---

  auto *fillAction   = menu->addAction("Set fill");
  auto *strokeAction = menu->addAction("Set stroke");

  connect(fillAction, SIGNAL(triggered()), this, SLOT(fillSlot()));
  connect(strokeAction, SIGNAL(triggered()), this, SLOT(strokeSlot()));

  //---

  (void) menu->exec(e->globalPos());

  delete menu;
}

void
ColorBar::
fillSlot()
{
  setFill(menuInd_);
}

void
ColorBar::
strokeSlot()
{
  setStroke(menuInd_);
}

void
ColorBar::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  const auto &palette = palettes_[paletteNum_];

  int n = palette.numColors() + 1;

  swatchWidth_ = width()/n;

  double x = 0.0;

  for (int i = 0; i < n; ++i) {
    auto rect = QRectF(x, 0, swatchWidth_, height());

    if (i == 0) {
      // transparent
      painter.setBrush(Qt::white);
    }
    else {
      const auto &c = paletteColor(i - 1);

      painter.setBrush(c);
    }

    painter.fillRect(rect, painter.brush());

    if (i == 0) {
      QPen pen;

      pen.setColor(Qt::red);
      pen.setWidth(4);

      painter.setPen(pen);

      painter.drawLine(rect.topLeft(), rect.bottomRight());
      painter.drawLine(rect.bottomLeft(), rect.topRight());
    }

    x += swatchWidth_;
  }
}

QColor
ColorBar::
paletteColor(int i) const
{
  const auto &palette = palettes_[paletteNum_];

  return palette.color(i);
}

}
