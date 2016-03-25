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

  double          real;
  std::string     str;
  CSVGLengthValue length;

  if      (svg_.coordOption (opt_name, opt_value, "cx"       , &real))
    cx_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "cy"       , &real))
    cy_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "r"        , length))
    radius_ = length.value();
  else if (svg_.stringOption(opt_name, opt_value, "transform", str)) {
    CMatrix2D transform;

    if (! svg_.decodeTransform(str, transform))
      return false;

    setTransform(transform);
  }
  else
    return false;

  return true;
}

void
CSVGCircle::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.drawCircle(getCenterX(), getCenterY(), getRadius());
}

bool
CSVGCircle::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isSet())
    bbox = CBBox2D(getCenterX() - getRadius(), getCenterY() - getRadius(),
                   getCenterX() + getRadius(), getCenterY() + getRadius());
  else
    bbox = viewBox_;

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
    os << "<circle";

    printNameValue(os, "id", id_);

    printNameValue(os, "cx", cx_);
    printNameValue(os, "cy", cy_);
    printNameValue(os, "r" , radius_);

    printStyle(os);

    os << "/>" << std::endl;
  }
  else
    os << "circle " << getCenter() << " radius " << getRadius();
}

std::ostream &
operator<<(std::ostream &os, const CSVGCircle &circle)
{
  circle.print(os, false);

  return os;
}
