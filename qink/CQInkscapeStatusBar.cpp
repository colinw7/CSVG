#include <CQInkscapeStatusBar.h>
#include <CQInkscapeWindow.h>

#include <CSVGObject.h>

#include <CQUtil.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>

namespace CQInkscape {

StatusBar::
StatusBar(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("statusBar");

  auto *layout = CQUtil::makeLayout<QHBoxLayout>(this);

  // fill/stroke, layer, message, x, y (z)
  fillStroke_ = new FillStrokeStatus(window);
  msgLabel_   = CQUtil::makeWidget<QLabel>(this, "label");
  posLabel_   = new PosLabel(window);

  layout->addWidget(fillStroke_);
  layout->addWidget(msgLabel_);
  layout->addWidget(posLabel_);
}

void
StatusBar::
updateCurrent()
{
  fillStroke_->update();

  //---

  auto fileName = window_->currentFile();

  if (fileName != "")
    window_->setWindowTitle(QString("CQInkscape : %1").arg(fileName));
  else
    window_->setWindowTitle("CQInkscape");

  //--

  QString msg;

  auto *obj = window_->currentObject();

  if (obj)
    msg += QString("%1:%2").arg(QString::fromStdString(obj->getTagName())).
                            arg(QString::fromStdString(obj->getId()));

  msgLabel_->setText(msg);
}

void
StatusBar::
setMousePos(const CPoint2D &p)
{
  posLabel_->setPos(p);
}

//---

FillStrokeStatus::
FillStrokeStatus(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("fillStroke");

  QFont font;

  font.setPointSizeF(font.pointSizeF()/1.5);

  setFont(font);

  setFixedWidth(128);
}

void
FillStrokeStatus::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  QFontMetricsF fm(painter.font());

  int x = 2;
  int y = 2;

  painter.drawText(x, y + fm.ascent(), "Fill:"  ); y += fm.height();
  painter.drawText(x, y + fm.ascent(), "Stroke:");

  x += fm.width("Stroke:") + 4;
  y  = 2;

  auto fh = fm.height();

  auto *obj = window_->currentObject();

  if (obj) {
    int xf = x;

    auto fill = obj->getFlatFill();

    CSVGColor fillColor;

    if (fill.getColorValid())
      fillColor = fill.getColor().getValue();

    if (fillColor.isRGBA()) {
      auto qcolor = CQUtil::rgbaToColor(fillColor.rgba());

      double fillOpacity = 1.0;

      if (fill.getOpacityValid())
        fillOpacity = fill.getOpacity().getValue();

      qcolor.setAlphaF(fillOpacity);

      auto frect = QRect(xf, y, fh, fh);

      painter.fillRect(frect, QBrush(qcolor));

      xf += fh + 2;
    }
    else {
      painter.drawText(xf, y + fm.ascent(), "None");

      xf += fm.width("None") + 2;
    }

    //---

    y += fh;

    int xs = x;

    auto stroke = obj->getFlatStroke();

    CSVGColor strokeColor;

    if (stroke.getColorValid())
      strokeColor = stroke.getColor().getValue();

    if (strokeColor.isRGBA()) {
      auto qcolor = CQUtil::rgbaToColor(strokeColor.rgba());

      double strokeOpacity = 1.0;

      if (stroke.getOpacityValid())
        strokeOpacity = stroke.getOpacity().getValue();

      qcolor.setAlphaF(strokeOpacity);

      auto frect = QRect(xs, y, fh, fh);

      painter.fillRect(frect, QBrush(qcolor));

      if (stroke.getWidthValid()) {
        auto width = stroke.getWidth().getValue();

        xs += fh + 2;

        auto widthStr = QString::number(width);

        painter.drawText(xs, y + fm.ascent(), widthStr);

        xs += fm.width(widthStr) + 2;
      }
    }
    else {
      painter.drawText(xs, y + fm.ascent(), "None");

      xs += fm.width("None");
    }

    width_ = std::max(xf, xs);
  }
  else {
    painter.drawText(x, y + fm.ascent(), "N/A"); y += fm.height();
    painter.drawText(x, y + fm.ascent(), "N/A");

    x += fm.width("N/A");

    width_ = x;
  }
}

QSize
FillStrokeStatus::
sizeHint() const
{
  QFontMetricsF fm(this->font());

  return QSize(std::max(width_, 100), 2*fm.height());
}

//---

PosLabel::
PosLabel(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("posLabel");

  QFont font;

  font.setPointSizeF(font.pointSizeF()/1.5);

  setFont(font);
}

void
PosLabel::
setPos(const CPoint2D &p)
{
  p_ = p;

  update();
}

void
PosLabel::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  QFontMetricsF fm(painter.font());

  int x = 2;
  int y = 2 + fm.ascent();

  painter.drawText(x, y, QString("X: %1").arg(p_.x));

  y += fm.height();

  painter.drawText(x, y, QString("Y: %1").arg(p_.y));
}

}
