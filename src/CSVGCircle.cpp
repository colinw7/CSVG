#include <CSVGCircle.h>
#include <CSVG.h>
#include <CSVGLog.h>

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Paint>
    <Color>
    <Opacity>
    <Graphics>
    <Clip>
    <Mask>
    <Filter>
    <GraphicalEvents>
    <Cursor>
    <External>
    cx        <Coordinate>
    cy        <Coordinate>
    r         <Length>
    transform <TransformList>
*/

CSVGCircle::
CSVGCircle(CSVG &svg) :
 CSVGObject(svg)
{
  init();
}

CSVGCircle::
CSVGCircle(const CSVGCircle &circle) :
 CSVGObject(circle),
 cx_       (circle.cx_),
 cy_       (circle.cy_),
 radius_   (circle.radius_)
{
  init();
}

void
CSVGCircle::
init()
{
  skipNames_.insert("cx");
  skipNames_.insert("cy");
  skipNames_.insert("r");

  updateBBox();
}

CSVGCircle *
CSVGCircle::
dup() const
{
  return new CSVGCircle(*this);
}

CPoint2D
CSVGCircle::
getCenter() const
{
  double xc = getCenterX().pxValue(CScreenUnits(1));
  double yc = getCenterY().pxValue(CScreenUnits(1));

  return CPoint2D(xc, yc);
}

void
CSVGCircle::
setCenter(const CPoint2D &center)
{
  setCenterX(CScreenUnits(center.x), /*update*/false);
  setCenterY(CScreenUnits(center.y));
}

bool
CSVGCircle::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processPaintOption          (opt_name, opt_value)) return true;
  if (processColorOption          (opt_name, opt_value)) return true;
  if (processOpacityOption        (opt_name, opt_value)) return true;
  if (processGraphicsOption       (opt_name, opt_value)) return true;
  if (processClipOption           (opt_name, opt_value)) return true;
  if (processMaskOption           (opt_name, opt_value)) return true;
  if (processFilterOption         (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processCursorOption         (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  std::string    str;
  CScreenUnits   length;
  CMatrixStack2D transform;

  if      (svg_.coordOption    (opt_name, opt_value, "cx"       , length))
    cx_ = length;
  else if (svg_.coordOption    (opt_name, opt_value, "cy"       , length))
    cy_ = length;
  else if (svg_.lengthOption   (opt_name, opt_value, "r"        , length))
    radius_ = length;
  else if (svg_.transformOption(opt_name, opt_value, "transform", transform))
    setTransform(transform);
  else
    return false;

  updateBBox();

  return true;
}

void
CSVGCircle::
updateBBox()
{
  // stroke width ?

  double xc = getCenterX().pxValue(CScreenUnits(1));
  double yc = getCenterY().pxValue(CScreenUnits(1));
  double r  = getRadius ().pxValue(CScreenUnits(1));

  bbox_ = CBBox2D(xc - r, yc - r, xc + r, yc + r);

  parts_ = CSVGPathPartList();
}

COptString
CSVGCircle::
getNameValue(const std::string &name) const
{
  COptString str;

  auto bbox = getDrawBBox();

  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  if      (name == "cx")
    str = CStrUtil::toString(getCenterX().pxValue(CScreenUnits(w)));
  else if (name == "cy")
    str = CStrUtil::toString(getCenterY().pxValue(CScreenUnits(h)));
  else if (name == "r")
    str = CStrUtil::toString(getRadius ().pxValue(CScreenUnits(w)));
  else
    str = CSVGObject::getNameValue(name);

  return str;
}

bool
CSVGCircle::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  auto drawBox = getDrawBBox();

  double dw = drawBox.getWidth ();
  double dh = drawBox.getHeight();

  double xc = getCenterX().pxValue(CScreenUnits(dw));
  double yc = getCenterY().pxValue(CScreenUnits(dh));
  double r  = getRadius ().pxValue(CScreenUnits(dw));

  // skip zero radius
  if (r <= 0)
    return false;

  //---

  if (svg_.isCheckViewBox()) {
    CBBox2D fbbox;

    getFlatTransformedBBox(fbbox);

    if (! fbbox.overlaps(drawBox))
      CSVGLog() << "Outside viewbox : " << *this;
  }

  //---

  svg_.drawCircle(xc, yc, r);

  return true;
}

bool
CSVGCircle::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox())
    bbox = bbox_;
  else
    bbox = getViewBox();

  return true;
}

const CSVGPathPartList &
CSVGCircle::
getPartList() const
{
  if (parts_.empty()) {
    auto drawBox = getDrawBBox();

    auto dc = drawBox.getCenter();
    auto dw = drawBox.getWidth ();

    double x = getCenterX().pxValue(dc.x);
    double y = getCenterY().pxValue(dc.y);
    double r = getRadius ().pxValue(dw/2.0);

    double fa = 1;
    double fs = 0;

    parts_.push_back(svg_.createPathMoveTo(x + r, y));
    parts_.push_back(svg_.createPathArcTo (r, r, M_PI, fa, fs, x - r, y));
    parts_.push_back(svg_.createPathArcTo (r, r, M_PI, fa, fs, x + r, y));
    parts_.push_back(svg_.createPathClosePath());
  }

  return parts_;
}

void
CSVGCircle::
moveBy(const CVector2D &delta)
{
  auto c = getCenter();

  c += delta;

  setCenter(c);
}

void
CSVGCircle::
resizeTo(const CSize2D &size)
{
  double r = std::min(size.getWidth(), size.getHeight())/2.0;

  setRadius(CScreenUnits(r));
}

void
CSVGCircle::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    CSVGObject::print(os, hier);
  }
  else
    os << "circle (" << getId() << ") center " <<
          getCenter() << " radius " << getRadius();
}

void
CSVGCircle::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue(os, "cx", cx_);
  printNameValue(os, "cy", cy_);
  printNameValue(os, "r" , radius_);
}
