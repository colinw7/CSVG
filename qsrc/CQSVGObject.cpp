#include <CQSVGObject.h>
#include <CQSVG.h>
#include <CQSVGWindow.h>
#include <CQSVGCanvas.h>
#include <CQSVGFontObj.h>
#include <CQSVGRenderer.h>
#include <CSVGLinearGradient.h>
#include <CSVGRadialGradient.h>
#include <CSVGObject.h>
#include <CSVGFilter.h>

#include <CQFontUtil.h>
#include <CQUtil.h>
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

double
CQSVGObject::
getDrawOpacity() const
{
  return obj_->getOpacity();
}

void
CQSVGObject::
setDrawOpacity(double r)
{
  obj_->setOpacity(r);
}

bool
CQSVGObject::
strokeIsNoColor() const
{
  return obj_->getStroke().getColor().isNone();
}

void
CQSVGObject::
setStrokeIsNoColor(bool b)
{
  CSVGStroke stroke = obj_->getStroke();

  if (b)
    stroke.setColor(CSVGColor(CSVGColor::Type::NONE));

  obj_->setStroke(stroke);
}

bool
CQSVGObject::
strokeIsCurrentColor() const
{
  return obj_->getStroke().getColor().isCurrent();
}

void
CQSVGObject::
setStrokeIsCurrentColor(bool b)
{
  CSVGStroke stroke = obj_->getStroke();

  if (b)
    stroke.setColor(CSVGColor(CSVGColor::Type::CURRENT));

  obj_->setStroke(stroke);
}

QColor
CQSVGObject::
strokeColor() const
{
  if (obj_->getStroke().getColor().isRGBA())
    return CQUtil::rgbaToColor(obj_->getStroke().getColor().rgba());
  else
    return QColor();
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
  return obj_->getFlatStrokeOpacity().getValue(1);
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
  return obj_->getFlatStrokeWidth().getValue(1);
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
  return obj_->getFlatStrokeLineDash().getValue(CSVGStrokeDash()).getLineDash();
}

void
CQSVGObject::
setStrokeDash(const CLineDash &dash)
{
  CSVGStrokeDash sdash = obj_->getStrokeDash();

  CSVGStrokeDash::Dashes dashes;

  for (uint i = 0; i < dash.getNumLengths(); ++i)
    dashes.push_back(dash.getLength(i));

  sdash.setDashes(dashes);

  obj_->setStrokeDash(sdash);
}

double
CQSVGObject::
strokeOffset() const
{
  return obj_->getFlatStrokeLineDash().getValue(CSVGStrokeDash()).offset().pxValue(1);
}

void
CQSVGObject::
setStrokeOffset(double o)
{
  CSVGStrokeDash sdash = obj_->getStrokeDash();

  sdash.setOffset(o);

  obj_->setStrokeDash(sdash);
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
  return obj_->getFill().getColor().isNone();
}

void
CQSVGObject::
setFillIsNoColor(bool b)
{
  CSVGFill fill = obj_->getFill();

  if (b)
    fill.setColor(CSVGColor(CSVGColor::Type::NONE));

  obj_->setFill(fill);
}

bool
CQSVGObject::
fillIsCurrentColor() const
{
  return obj_->getFill().getColor().isCurrent();
}

void
CQSVGObject::
setFillIsCurrentColor(bool b)
{
  CSVGFill fill = obj_->getFill();

  if (b)
    fill.setColor(CSVGColor(CSVGColor::Type::CURRENT));

  obj_->setFill(fill);
}

QColor
CQSVGObject::
fillColor() const
{
  if (obj_->getFill().getColor().isRGBA())
    return CQUtil::rgbaToColor(obj_->getFill().getColor().rgba());
  else
    return QColor();
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
  return obj_->getFlatFillOpacity().getValue(1);
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
  return obj_->getFlatFontSize().px().value();
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
  CSVGFontDef fontDef = obj_->getFlatFontDef();

  CQSVGFontObj *fontObj = dynamic_cast<CQSVGFontObj *>(fontDef.getObj());

  return fontObj->font();
}

void
CQSVGObject::
setFont(QFont f)
{
  CFontPtr font = CQFontUtil::fromQFont(f);

  CSVGFontDef fontDef = obj_->getFontDef();

  //CQSVGFontObj *fontObj = dynamic_cast<CQSVGFontObj *>(fontDef.getObj());

  //fontDef.setFont(font);

  obj_->setFontDef(fontDef);
}

void
CQSVGObject::
drawSelected()
{
  if (! isSelected())
    return;

  CQSVGCanvas *canvas = qsvg_->window()->canvas();

  //---

  CBBox2D bbox;

  if (obj_->getFlatTransformedBBox(bbox)) {
    double xscale = obj_->getSVG().xscale();
    double yscale = obj_->getSVG().yscale();

    CPoint2D p1(xscale*bbox.getXMin(), yscale*bbox.getYMin());
    CPoint2D p2(xscale*bbox.getXMax(), yscale*bbox.getYMax());

    CPoint2D w1, w2;

    canvas->pixelToWindow(p1, w1);
    canvas->pixelToWindow(p2, w2);

    canvas->drawRect(CBBox2D(w1, w2), Qt::red);
  }

  //---

  if (qsvg_->isShowFilterBox()) {
    CSVGFilter *filter = obj_->getFilter();

    if (filter) {
      const CBBox2D &bbox = filter->getContentsBBox();

      double xscale = obj_->getSVG().xscale();
      double yscale = obj_->getSVG().yscale();

      CPoint2D p1(xscale*bbox.getXMin(), yscale*bbox.getYMin());
      CPoint2D p2(xscale*bbox.getXMax(), yscale*bbox.getYMax());

      CPoint2D w1, w2;

      canvas->pixelToWindow(p1, w1);
      canvas->pixelToWindow(p2, w2);

      canvas->drawRect(CBBox2D(w1, w2), Qt::red);
    }
  }

  //---

  if (qsvg_->isShowGradient()) {
    const CSVGFill &fill = obj_->getFill();

    CSVGObject *fillObj = fill.getFillObject();

    CSVGLinearGradient *lgrad = dynamic_cast<CSVGLinearGradient *>(fillObj);
    CSVGRadialGradient *rgrad = dynamic_cast<CSVGRadialGradient *>(fillObj);

    QColor bg(Qt::white);
    QColor fg(Qt::blue);

    if      (lgrad) {
      double x1, y1, x2, y2;

      lgrad->getEndPoints(obj_, &x1, &y1, &x2, &y2);

      CPoint2D p1(x1, y1);
      CPoint2D p2(x2, y2);

      CMatrixStack2D m = obj_->getFlatTransform();

      m.multiplyPoint(p1.x, p1.y, &x1, &y1);
      m.multiplyPoint(p2.x, p2.y, &x2, &y2);

      p1 = CPoint2D(x1, y1);
      p2 = CPoint2D(x2, y2);

      canvas->drawLine(p1, p2, fg);

      canvas->drawPoints({p1, p2}, CQSVGCanvas::Shape::CIRCLE, 4, bg, fg);

      // TODO: draw stops
    }
    else if (rgrad) {
      double xc, yc, r, xf, yf;

      rgrad->getControlPoints(obj_, &xc, &yc, &r, &xf, &yf);

      CPoint2D pc(xc, yc);
      CPoint2D p1(xc + r, yc);
      CPoint2D p2(xc, yc + r);
      CPoint2D pf(xf, yf);

      CMatrixStack2D m = obj_->getFlatTransform();

      double x1, y1, x2, y2;

      m.multiplyPoint(pc.x, pc.y, &xc, &yc);
      m.multiplyPoint(p1.x, p1.y, &x1, &y1);
      m.multiplyPoint(p2.x, p2.y, &x2, &y2);
      m.multiplyPoint(pf.x, pf.y, &xf, &yf);

      pc = CPoint2D(xc, yc);
      p1 = CPoint2D(x1, y1);
      p2 = CPoint2D(x2, y2);
      pf = CPoint2D(xf, yf);

      canvas->drawLine(pc, p1, fg);
      canvas->drawLine(pc, p2, fg);

      canvas->drawPoints({pc, p1, p2, pf}, CQSVGCanvas::Shape::CIRCLE, 4, bg, fg);

      // TODO: draw stops
    }
  }
}
