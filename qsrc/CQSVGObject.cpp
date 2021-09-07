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

#include <CQPropertyTree.h>
#include <CQPropertyItem.h>
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
  return obj_->getOpacity().getValue();
}

void
CQSVGObject::
setDrawOpacity(double r)
{
  obj_->setOpacity(CSVGObject::Opacity(r));
}

bool
CQSVGObject::
strokeIsNoColor() const
{
  return obj_->getStroke().getColor().getValue().isNone();
}

void
CQSVGObject::
setStrokeIsNoColor(bool b)
{
  auto stroke = obj_->getStroke();

  if (b)
    stroke.setColor(Color(CSVGColor(CSVGColor::Type::NONE)));

  obj_->setStroke(stroke);
}

bool
CQSVGObject::
strokeIsCurrentColor() const
{
  return obj_->getStroke().getColor().getValue().isCurrent();
}

void
CQSVGObject::
setStrokeIsCurrentColor(bool b)
{
  auto stroke = obj_->getStroke();

  if (b)
    stroke.setColor(Color(CSVGColor(CSVGColor::Type::CURRENT)));

  obj_->setStroke(stroke);
}

QColor
CQSVGObject::
strokeColor() const
{
  if (obj_->getStroke().getColor().getValue().isRGBA())
    return CQUtil::rgbaToColor(obj_->getStroke().getColor().getValue().rgba());
  else
    return QColor();
}

void
CQSVGObject::
setStrokeColor(const QColor &c)
{
  obj_->setStrokeColor(Color(CQUtil::colorToRGBA(c)));
}

double
CQSVGObject::
strokeOpacity() const
{
  return obj_->getFlatStrokeOpacity().getValue(CSVGObject::Opacity(1)).getValue();
}

void
CQSVGObject::
setStrokeOpacity(double r)
{
  obj_->setStrokeOpacity(CSVGObject::Opacity(r));
}

double
CQSVGObject::
strokeWidth() const
{
  return obj_->getFlatStrokeWidth().getValue(CSVGObject::Width(1)).getValue(0);
}

void
CQSVGObject::
setStrokeWidth(double r)
{
  obj_->setStrokeWidth(CSVGObject::Width(r));
}

CLineDash
CQSVGObject::
strokeDash() const
{
  auto sdash = obj_->getFlatStrokeDashArray().getValue(CSVGObject::DashArray()).getValue();

  CLineDash dash;

  if (sdash.isDashed()) {
    std::vector<double> lengths;

    for (int i = 0; i < sdash.numDashes(); ++i)
      lengths.push_back(sdash.dash(i).pxValue());

    dash = CLineDash(lengths);
  }

  return dash;
}

void
CQSVGObject::
setStrokeDash(const CLineDash &dash)
{
  auto sdash = obj_->getStrokeDashArray().getValue();

  CSVGLineDash::Dashes dashes;

  for (uint i = 0; i < dash.getNumLengths(); ++i)
    dashes.push_back(dash.getLength(i));

  sdash.setDashes(dashes);

  obj_->setStrokeDashArray(CSVGObject::DashArray(sdash));
}

double
CQSVGObject::
strokeOffset() const
{
  auto dash = obj_->getFlatStrokeDashOffset().getValue(CSVGObject::DashOffset(0)).getValue();

  return dash.pxValue(0);
}

void
CQSVGObject::
setStrokeOffset(double o)
{
  obj_->setStrokeDashOffset(CSVGObject::DashOffset(CScreenUnits(o)));
}

CQSVGEnum::LineCapType
CQSVGObject::
strokeCap() const
{
  return CQSVGEnum::lineCapConv(obj_->getStrokeLineCap().getValue());
}

void
CQSVGObject::
setStrokeCap(const CQSVGEnum::LineCapType &a)
{
  obj_->setStrokeLineCap(CSVGObject::LineCap(CQSVGEnum::lineCapConv(a)));
}

CQSVGEnum::LineJoinType
CQSVGObject::
strokeJoin() const
{
  return CQSVGEnum::lineJoinConv(obj_->getStrokeLineJoin().getValue());
}

void
CQSVGObject::
setStrokeJoin(const CQSVGEnum::LineJoinType &a)
{
  obj_->setStrokeLineJoin(CSVGObject::LineJoin(CQSVGEnum::lineJoinConv(a)));
}

bool
CQSVGObject::
fillIsNoColor() const
{
  return obj_->getFill().getColor().getValue().isNone();
}

void
CQSVGObject::
setFillIsNoColor(bool b)
{
  auto fill = obj_->getFill();

  if (b)
    fill.setColor(Color(CSVGColor(CSVGColor::Type::NONE)));

  obj_->setFill(fill);
}

bool
CQSVGObject::
fillIsCurrentColor() const
{
  return obj_->getFill().getColor().getValue().isCurrent();
}

void
CQSVGObject::
setFillIsCurrentColor(bool b)
{
  auto fill = obj_->getFill();

  if (b)
    fill.setColor(Color(CSVGColor(CSVGColor::Type::CURRENT)));

  obj_->setFill(fill);
}

QColor
CQSVGObject::
fillColor() const
{
  if (obj_->getFill().getColor().getValue().isRGBA())
    return CQUtil::rgbaToColor(obj_->getFill().getColor().getValue().rgba());
  else
    return QColor();
}

void
CQSVGObject::
setFillColor(const QColor &c)
{
  auto fill = obj_->getFill();

  fill.setColor(Color(CQUtil::colorToRGBA(c)));

  obj_->setFill(fill);
}

double
CQSVGObject::
fillOpacity() const
{
  return obj_->getFlatFillOpacity().getValue(CSVGObject::Opacity(1)).getValue();
}

void
CQSVGObject::
setFillOpacity(double r)
{
  obj_->setFillOpacity(CSVGObject::Opacity(r));
}

CQSVGEnum::FillType
CQSVGObject::
fillRule() const
{
  return CQSVGEnum::fillTypeConv(obj_->getFill().getRule().getValue());
}

void
CQSVGObject::
setFillRule(const CQSVGEnum::FillType &r)
{
  auto fill = obj_->getFill();

  fill.setRule(CSVGFill::FillType(CQSVGEnum::fillTypeConv(r)));

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
  auto fill = obj_->getFill();

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
  return obj_->getFlatFontFamily().getValue().getValue().c_str();
}

void
CQSVGObject::
setFontFamily(const QString &str)
{
  obj_->setFontFamily(CSVGObject::FontFamily(str.toStdString()));
}

double
CQSVGObject::
getFontSize() const
{
  return obj_->getFlatFontSize().getValue().getValue().px().value();
}

void
CQSVGObject::
setFontSize(double s)
{
  obj_->setFontSize(CSVGObject::FontSize(CScreenUnits(s)));
}

QFont
CQSVGObject::
getFont() const
{
  auto fontDef = obj_->getFlatFontDef();

  auto *fontObj = dynamic_cast<CQSVGFontObj *>(fontDef.getObj());

  return fontObj->qfont();
}

void
CQSVGObject::
setFont(QFont f)
{
  auto fontDef = obj_->getFontDef();

  auto *fontObj = dynamic_cast<CQSVGFontObj *>(fontDef.getObj());

  fontObj->setQFont(f);
}

void
CQSVGObject::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  auto objName = QString::fromStdString(name);

  propTree->addProperty(objName, this, "id");

  //---

  bool hasChildren = object()->hasChildren();

  if (object()->isDrawable() || hasChildren)
    propTree->addProperty(objName, this, "visible");

  if (object()->getFilter())
    propTree->addProperty(objName, this, "filtered");

  if (object()->getClipPath())
    propTree->addProperty(objName, this, "clipped");

  if (object()->getMask())
    propTree->addProperty(objName, this, "masked");

  if (! object()->isAnimated()) {
    if (object()->isDrawable() || hasChildren)
      propTree->addProperty(objName, this, "transform");

    if (object()->isDrawable() || hasChildren) {
      propTree->addProperty(objName, this, "opacity");

      //---

      auto strokeName = objName + "/stroke";

      propTree->addProperty(strokeName, this, "strokeNoColor"     )->setLabel("noColor"     );
      propTree->addProperty(strokeName, this, "strokeCurrentColor")->setLabel("currentColor");
      propTree->addProperty(strokeName, this, "strokeColor"       )->setLabel("color"       );
      propTree->addProperty(strokeName, this, "strokeOpacity"     )->setLabel("opacity"     );
      propTree->addProperty(strokeName, this, "strokeWidth"       )->setLabel("width"       );
      propTree->addProperty(strokeName, this, "strokeDash"        )->setLabel("dash"        );
      propTree->addProperty(strokeName, this, "strokeOffset"      )->setLabel("offset"      );
      propTree->addProperty(strokeName, this, "strokeCap"         )->setLabel("cap"         );
      propTree->addProperty(strokeName, this, "strokeJoin"        )->setLabel("join"        );

      //---

      auto fillName = objName + "/fill";

      propTree->addProperty(fillName, this, "fillNoColor"     )->setLabel("noColor"     );
      propTree->addProperty(fillName, this, "fillCurrentColor")->setLabel("currentColor");
      propTree->addProperty(fillName, this, "fillColor"       )->setLabel("color"       );
      propTree->addProperty(fillName, this, "fillOpacity"     )->setLabel("opacity"     );
      propTree->addProperty(fillName, this, "fillRule"        )->setLabel("rule"        );
      propTree->addProperty(fillName, this, "fillUrl"         )->setLabel("url"         );
    }

    if (object()->hasFont()) {
      auto fontName = objName + "/font";

      propTree->addProperty(fontName, this, "fontFamily")->setLabel("family");
    //propTree->addProperty(fontName, this, "fontStyle" )->setLabel("style");
      propTree->addProperty(fontName, this, "fontSize"  )->setLabel("size");
      propTree->addProperty(fontName, this, "font"      )->setLabel("font");
    }
  }
}

void
CQSVGObject::
drawSelected()
{
  if (! isSelected())
    return;

  auto *canvas = qsvg_->window()->canvas();

  //---

  CBBox2D bbox;

  if (obj_->getFlatTransformedBBox(bbox)) {
    double xscale = obj_->getSVG().xscale();
    double yscale = obj_->getSVG().yscale();

    CPoint2D p1(xscale*bbox.getXMin(), yscale*bbox.getYMin());
    CPoint2D p2(xscale*bbox.getXMax(), yscale*bbox.getYMax());

    auto w1 = canvas->pixelToWindow(p1);
    auto w2 = canvas->pixelToWindow(p2);

    canvas->drawRect(CBBox2D(w1, w2), Qt::red);
  }

  //---

  if (qsvg_->isShowFilterBox()) {
    auto *filter = obj_->getFilter();

    if (filter) {
      const auto &bbox = filter->getContentsBBox();

      double xscale = obj_->getSVG().xscale();
      double yscale = obj_->getSVG().yscale();

      CPoint2D p1(xscale*bbox.getXMin(), yscale*bbox.getYMin());
      CPoint2D p2(xscale*bbox.getXMax(), yscale*bbox.getYMax());

      auto w1 = canvas->pixelToWindow(p1);
      auto w2 = canvas->pixelToWindow(p2);

      canvas->drawRect(CBBox2D(w1, w2), Qt::red);
    }
  }

  //---

  if (qsvg_->isShowGradient()) {
    const auto &fill = obj_->getFill();

    auto *fillObj = fill.getFillObject();

    auto *lgrad = dynamic_cast<CSVGLinearGradient *>(fillObj);
    auto *rgrad = dynamic_cast<CSVGRadialGradient *>(fillObj);

    QColor bg(Qt::white);
    QColor fg(Qt::blue);

    if      (lgrad) {
      double x1, y1, x2, y2;

      lgrad->getEndPoints(obj_, &x1, &y1, &x2, &y2);

      CPoint2D p1(x1, y1);
      CPoint2D p2(x2, y2);

      auto m = obj_->getFlatTransform();

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

      auto m = obj_->getFlatTransform();

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
