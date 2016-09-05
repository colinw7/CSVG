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
}

CSVGCircle::
CSVGCircle(const CSVGCircle &circle) :
 CSVGObject(circle),
 cx_       (circle.cx_),
 cy_       (circle.cy_),
 radius_   (circle.radius_)
{
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
  double xc = getCenterX().pxValue(1);
  double yc = getCenterY().pxValue(1);

  return CPoint2D(xc, yc);
}

void
CSVGCircle::
setCenter(const CPoint2D &center)
{
  setCenterX(center.x);
  setCenterY(center.y);
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

  return true;
}

COptString
CSVGCircle::
getNameValue(const std::string &name) const
{
  COptString str;

  CBBox2D bbox = getDrawBBox();

  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  if      (name == "cx")
    str = CStrUtil::toString(getCenterX().pxValue(w));
  else if (name == "cy")
    str = CStrUtil::toString(getCenterY().pxValue(h));
  else if (name == "r")
    str = CStrUtil::toString(getRadius ().pxValue(w));
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

  CBBox2D bbox = getDrawBBox();

  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  double xc = getCenterX().pxValue(w);
  double yc = getCenterY().pxValue(h);
  double r  = getRadius ().pxValue(w);

  if (r <= 0)
    return false;

  svg_.drawCircle(xc, yc, r);

  return true;
}

bool
CSVGCircle::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox()) {
    double xc = getCenterX().pxValue(1);
    double yc = getCenterY().pxValue(1);
    double r  = getRadius ().pxValue(1);

    bbox = CBBox2D(xc - r, yc - r, xc + r, yc + r);
  }
  else
    bbox = getViewBox();

  return true;
}

void
CSVGCircle::
moveBy(const CVector2D &delta)
{
  CPoint2D c = getCenter();

  c += delta;

  setCenter(c);
}

void
CSVGCircle::
resizeTo(const CSize2D &size)
{
  setRadius(std::min(size.getWidth(), size.getHeight())/2.0);
}

void
CSVGCircle::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    CSVGObject::print(os, hier);
  }
  else
    os << "circle " << getCenter() << " radius " << getRadius();
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

std::ostream &
operator<<(std::ostream &os, const CSVGCircle &circle)
{
  circle.print(os, false);

  return os;
}
