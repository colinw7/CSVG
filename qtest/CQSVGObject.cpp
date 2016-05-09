#include <CQSVGObject.h>
#include <CQSVG.h>
#include <CQUtil.h>
#include <CQSVGWindow.h>
#include <CQSVGCanvas.h>
#include <CQSVGRenderer.h>
#include <CSVGObject.h>
#include <QPainter>

CQSVGObject::
CQSVGObject(CQSVG *qsvg, CSVGObject *obj) :
 qsvg_(qsvg), obj_(obj)
{
}

QString
CQSVGObject::
id() const
{
  return QString(obj_->getId().c_str());
}

void
CQSVGObject::
setId(const QString &id)
{
  obj_->setId(id.toStdString());
}

bool
CQSVGObject::
isFiltered() const
{
  return obj_->getFiltered();
}

void
CQSVGObject::
setFiltered(bool b)
{
  obj_->setFiltered(b);
}

bool
CQSVGObject::
isClipped() const
{
  return obj_->getClipped();
}

void
CQSVGObject::
setClipped(bool b)
{
  obj_->setClipped(b);
}

bool
CQSVGObject::
isMasked() const
{
  return obj_->getMasked();
}

void
CQSVGObject::
setMasked(bool b)
{
  obj_->setMasked(b);
}

bool
CQSVGObject::
isVisible() const
{
  return obj_->isVisible();
}

void
CQSVGObject::
setVisible(bool b)
{
  obj_->setVisible(b);
}

QString
CQSVGObject::
transformStr() const
{
  return obj_->getTransform().getMatrix().toString().c_str();
}

QColor
CQSVGObject::
strokeColor() const
{
  return CQUtil::rgbaToColor(obj_->getStroke().getColor());
}

void
CQSVGObject::
setStrokeColor(const QColor &c)
{
  obj_->setStrokeColor(CQUtil::colorToRGBA(c));
}

double
CQSVGObject::
strokeOpacity() const
{
  return obj_->getFlatStrokeOpacity();
}

void
CQSVGObject::
setStrokeOpacity(double r)
{
  obj_->setStrokeOpacity(r);
}

double
CQSVGObject::
strokeWidth() const
{
  return obj_->getFlatStrokeWidth();
}

void
CQSVGObject::
setStrokeWidth(double r)
{
  obj_->setStrokeWidth(r);
}

CLineDash
CQSVGObject::
strokeDash() const
{
  return obj_->getFlatStrokeLineDash();
}

void
CQSVGObject::
setStrokeDash(const CLineDash &dash)
{
  obj_->setStrokeDash(dash);
}

CQSVGEnum::LineCapType
CQSVGObject::
strokeCap() const
{
  return CQSVGEnum::lineCapConv(obj_->getStrokeLineCap());
}

void
CQSVGObject::
setStrokeCap(const CQSVGEnum::LineCapType &a)
{
  obj_->setStrokeLineCap(CQSVGEnum::lineCapConv(a));
}

CQSVGEnum::LineJoinType
CQSVGObject::
strokeJoin() const
{
  return CQSVGEnum::lineJoinConv(obj_->getStrokeLineJoin());
}

void
CQSVGObject::
setStrokeJoin(const CQSVGEnum::LineJoinType &a)
{
  obj_->setStrokeLineJoin(CQSVGEnum::lineJoinConv(a));
}

bool
CQSVGObject::
fillIsNoColor() const
{
  return obj_->getFill().getNoColor();
}

void
CQSVGObject::
setFillIsNoColor(bool b)
{
  CSVGFill fill = obj_->getFill();

  fill.setNoColor(b);

  obj_->setFill(fill);
}

bool
CQSVGObject::
fillIsCurrentColor() const
{
  return obj_->getFill().isCurrentColor();
}

void
CQSVGObject::
setFillIsCurrentColor(bool b)
{
  CSVGFill fill = obj_->getFill();

  fill.setIsCurrentColor(b);

  obj_->setFill(fill);
}

QColor
CQSVGObject::
fillColor() const
{
  return CQUtil::rgbaToColor(obj_->getFill().getColor());
}

void
CQSVGObject::
setFillColor(const QColor &c)
{
  CSVGFill fill = obj_->getFill();

  fill.setColor(CQUtil::colorToRGBA(c));

  obj_->setFill(fill);
}

double
CQSVGObject::
fillOpacity() const
{
  return obj_->getFlatFillOpacity();
}

void
CQSVGObject::
setFillOpacity(double r)
{
  obj_->setFillOpacity(r);
}

CQSVGEnum::FillType
CQSVGObject::
fillRule() const
{
  return CQSVGEnum::fillTypeConv(obj_->getFill().getRule());
}

void
CQSVGObject::
setFillRule(const CQSVGEnum::FillType &r)
{
  CSVGFill fill = obj_->getFill();

  fill.setRule(CQSVGEnum::fillTypeConv(r));

  obj_->setFill(fill);
}

QString
CQSVGObject::
fillUrl() const
{
  return obj_->getFill().getUrl().c_str();
}

void
CQSVGObject::
setFillUrl(const QString &str)
{
  CSVGFill fill = obj_->getFill();

  if (str != "")
    fill.setUrl(str.toStdString());
  else
    fill.resetUrl();

  obj_->setFill(fill);
}

QString
CQSVGObject::
getFontFamily() const
{
  return obj_->getFlatFontFamily().c_str();
}

void
CQSVGObject::
setFontFamily(const QString &str)
{
  obj_->setFontFamily(str.toStdString());
}

double
CQSVGObject::
getFontSize() const
{
  return obj_->getFlatFontSize().value();
}

void
CQSVGObject::
setFontSize(double s)
{
  obj_->setFontSize(s);
}

QFont
CQSVGObject::
getFont() const
{
  return CQUtil::toQFont(obj_->getFont());
}

void
CQSVGObject::
setFont(QFont f)
{
  obj_->setFont(CQUtil::fromQFont(f));
}

void
CQSVGObject::
drawSelected()
{
  if (! isSelected())
    return;

  CBBox2D bbox;

  if (obj_->getFlatTransformedBBox(bbox)) {
    CQSVGCanvas *canvas = qsvg_->window()->canvas();

    double scale = obj_->getSVG().scale();

    CBBox2D bbox1 = scale*bbox;

    canvas->drawRect(bbox1, Qt::red);
  }
}
