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
  return obj_->getTransform().toString().c_str();
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
  return obj_->getStrokeOpacity();
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
  return obj_->getStrokeWidth();
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
  return obj_->getStrokeLineDash();
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
  return obj_->getFill().isNoColor();
}

void
CQSVGObject::
setFillIsNoColor(bool b)
{
  CSVGFill fill = obj_->getFill();

  fill.setIsNoColor(b);

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
  return obj_->getFillOpacity();
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

void
CQSVGObject::
drawSelected()
{
  if (! isSelected())
    return;

  CBBox2D bbox;

  if (obj_->getFlatTransformedBBox(bbox)) {
    CQSVGCanvas *canvas = qsvg_->window()->canvas();

    canvas->drawRect(bbox, Qt::red);
  }
}
