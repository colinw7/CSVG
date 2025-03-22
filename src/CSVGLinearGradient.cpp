#include <CSVGLinearGradient.h>
#include <CSVGRadialGradient.h>
#include <CSVGStop.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CLinearGradient.h>

/* Attributes:
    <Core>
    <Style>
    <Color>
    <Gradient>
    <XLink>
    <External>
    x1 <Coordinate>
    y1 <Coordinate>
    x2 <Coordinate>
    y2 <Coordinate>
    gradientUnits ( userSpaceOnUse | objectBoundingBox )
    gradientTransform <TransformList>
    spreadMethod ( pad | reflect | repeat )
*/
CSVGLinearGradient::
CSVGLinearGradient(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGLinearGradient::
CSVGLinearGradient(const CSVGLinearGradient &lg) :
 CSVGObject (lg),
 x1_        (lg.x1_),
 y1_        (lg.y1_),
 x2_        (lg.x2_),
 y2_        (lg.y2_),
 stops_     (lg.stops_),
 gtransform_(lg.gtransform_),
 units_     (lg.units_),
 spread_    (lg.spread_),
 xlink_     (lg.xlink_)
{
}

CSVGLinearGradient *
CSVGLinearGradient::
dup() const
{
  return new CSVGLinearGradient(*this);
}

bool
CSVGLinearGradient::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string    str;
  CSVGCoordUnits units;
  CScreenUnits   length;
  CMatrixStack2D transform;

  if      (svg_.percentOption(opt_name, opt_value, "x1", length))
    x1_ = length;
  else if (svg_.percentOption(opt_name, opt_value, "y1", length))
    y1_ = length;
  else if (svg_.percentOption(opt_name, opt_value, "x2", length))
    x2_ = length;
  else if (svg_.percentOption(opt_name, opt_value, "y2", length))
    y2_ = length;
  else if (svg_.transformOption(opt_name, opt_value, "gradientTransform", transform))
    setGTransform(transform);
  else if (svg_.coordUnitsOption(opt_name, opt_value, "gradientUnits", units))
    setUnits(units);
  else if (svg_.stringOption(opt_name, opt_value, "spreadMethod", str)) {
    CGradientSpreadType spread;

    if (! svg_.decodeGradientSpread(str, spread))
      return false;

    setSpread(spread);
  }
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str)) {
    xlink_ = CSVGXLink(this, str);

    addLinkStops();
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGLinearGradient::
termParse()
{
  std::vector<CSVGObject *> objects;

  getChildrenOfType(CSVGObjTypeId::STOP, objects);

  for (const auto &o : objects) {
    auto *stop = dynamic_cast<CSVGStop *>(o);

    if (stop != nullptr)
      addStop(stop);
  }
}

void
CSVGLinearGradient::
addLinkStops()
{
  auto *object = xlink_.getValue().getObject();

  if (! object) {
    //CSVGLog() << "No object for linear gradient link";
    return;
  }

  auto *rg = dynamic_cast<CSVGRadialGradient *>(object);
  auto *lg = dynamic_cast<CSVGLinearGradient *>(object);

  if      (lg) {
    if (lg->x1_.isValid()) x1_ = lg->x1_;
    if (lg->y1_.isValid()) y1_ = lg->y1_;
    if (lg->x2_.isValid()) x2_ = lg->x2_;
    if (lg->y2_.isValid()) y2_ = lg->y2_;

    if (lg->getGTransformValid()) gtransform_ = lg->getGTransform();
    if (lg->getUnitsValid     ()) units_      = lg->getUnits();
    if (lg->getSpreadValid    ()) spread_     = lg->getSpread();

    if (lg->anyStops()) {
      stops_.clear();

      for (const auto &s : lg->stops())
        addStop(s);
    }
  }
  else if (rg) {
    if (rg->getGTransformValid()) gtransform_ = rg->getGTransform();
    if (rg->getUnitsValid     ()) units_      = rg->getUnits();
    if (rg->getSpreadValid    ()) spread_     = rg->getSpread();

    if (rg->anyStops()) {
      stops_.clear();

      for (const auto &s : rg->stops())
        addStop(s);
    }
  }
  else {
    CSVGLog() << "Unexpected object type for linear gradient link";
  }
}

void
CSVGLinearGradient::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<linearGradient";

    printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</linearGradient>" << std::endl;
  }
  else
    os << "linearGradient ";
}

void
CSVGLinearGradient::
printValues(std::ostream &os, bool flat) const
{
  printNamePercent(os, "x1", x1_);
  printNamePercent(os, "y1", y1_);
  printNamePercent(os, "x2", x2_);
  printNamePercent(os, "y2", y2_);

  printNameTransform(os, "gradientTransform", gtransform_);

  if (getUnitsValid())
    os << " gradientUnits=\"" << CSVG::encodeUnitsString(getUnits()) << "\"";

  if (getSpreadValid())
    os << " spreadMethod=\"" << CSVG::encodeGradientSpread(getSpread()) << "\"";

  if (! xlink_.getValue().isNull())
    printNameXLink(os, "xlink:href", xlink_);

  CSVGObject::printValues(os, flat);
}

void
CSVGLinearGradient::
setFillBuffer(CSVGBuffer *buffer, CSVGObject *obj, const COptReal &opacity)
{
  using GradientP = std::unique_ptr<CLinearGradient>;

  auto lg = GradientP(createGradient(obj, opacity));

  buffer->setFillGradient(lg.get());
}

void
CSVGLinearGradient::
setStrokeBuffer(CSVGBuffer *buffer, CSVGObject *obj, const COptReal &opacity)
{
  using GradientP = std::unique_ptr<CLinearGradient>;

  auto lg = GradientP(createGradient(obj, opacity));

  buffer->setStrokeFillGradient(lg.get());
}

CLinearGradient *
CSVGLinearGradient::
createGradient(CSVGObject *obj, const COptReal &opacity)
{
  double x1, y1, x2, y2;

  getEndPoints(obj, &x1, &y1, &x2, &y2);

  double l = hypot(x2 - x1, y2 - y1);

  auto gradient = new CLinearGradient;

  gradient->setLine(x1, y1, x2, y2);

  if (spread_.isValid())
    gradient->setSpread(getSpread());

  for (const auto &s : stops()) {
    CRGBA rgba;

    if (hasCurrentColor())
      rgba = colorToRGBA(s->getColor());
    else
      rgba = obj->colorToRGBA(s->getColor());

    double alpha = 1.0;

    if (s->hasOpacity())
      alpha = s->getOpacity();
    else
      alpha = opacity.getValue(1.0);

    rgba.setAlpha(alpha);

    double sv = s->getOffset().ratioValue(l);

    gradient->addStop(sv, rgba);
  }

  gradient->init(1, 1);

  return gradient;
}

void
CSVGLinearGradient::
getEndPoints(CSVGObject *obj, double *x1, double *y1, double *x2, double *y2) const
{
  *x1 = getX1(0).pxValue(1);
  *y1 = getY1(0).pxValue(1);
  *x2 = getX2(1).pxValue(1);
  *y2 = getY2(0).pxValue(1);

  // remap points to object bounding box (will be mapped to flat coord by drawing code)
  if      (getUnits() == CSVGCoordUnits::USER_SPACE) {
  }
  else if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CPoint2D p1(*x1, *y1);
    CPoint2D p2(*x2, *y2);

    CBBox2D bbox;

    if (obj) {
      obj->getBBox(bbox);

      *x1 = CMathUtil::map(p1.x, 0, 1, bbox.getXMin(), bbox.getXMax());
      *y1 = CMathUtil::map(p1.y, 0, 1, bbox.getYMin(), bbox.getYMax());
      *x2 = CMathUtil::map(p2.x, 0, 1, bbox.getXMin(), bbox.getXMax());
      *y2 = CMathUtil::map(p2.y, 0, 1, bbox.getYMin(), bbox.getYMax());
    }
  }

  //---

  if (getGTransformValid()) {
    CPoint2D p1(*x1, *y1);
    CPoint2D p2(*x2, *y2);

    getGTransform().multiplyPoint(p1.x, p1.y, x1, y1);
    getGTransform().multiplyPoint(p2.x, p2.y, x2, y2);
  }
}
