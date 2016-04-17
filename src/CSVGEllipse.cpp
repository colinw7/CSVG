#include <CSVGEllipse.h>
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
    rx        <Length>
    ry        <Length>
    transform <TransformList>
*/

CSVGEllipse::
CSVGEllipse(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGEllipse::
CSVGEllipse(const CSVGEllipse &ellipse) :
 CSVGObject(ellipse),
 cx_       (ellipse.cx_),
 cy_       (ellipse.cy_),
 rx_       (ellipse.rx_),
 ry_       (ellipse.ry_)
{
}

CSVGEllipse *
CSVGEllipse::
dup() const
{
  return new CSVGEllipse(*this);
}

bool
CSVGEllipse::
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
  CMatrix2D       transform;

  if      (svg_.coordOption    (opt_name, opt_value, "cx", &real))
    cx_ = real;
  else if (svg_.coordOption    (opt_name, opt_value, "cy", &real))
    cy_ = real;
  else if (svg_.lengthOption   (opt_name, opt_value, "rx" , length))
    rx_ = length.value();
  else if (svg_.lengthOption   (opt_name, opt_value, "ry" , length))
    ry_ = length.value();
  else if (svg_.transformOption(opt_name, opt_value, "transform", transform))
    setTransform(transform);
  else
    return false;

  return true;
}

void
CSVGEllipse::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.drawEllipse(getCenterX(), getCenterY(), getRadiusX(), getRadiusY());
}

bool
CSVGEllipse::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid())
    bbox = CBBox2D(getCenterX() - getRadiusX(), getCenterY() - getRadiusY(),
                   getCenterX() + getRadiusX(), getCenterY() + getRadiusY());
  else
    bbox = getViewBox();

  return true;
}

void
CSVGEllipse::
moveBy(const CVector2D &delta)
{
  CPoint2D c = getCenter();

  c += delta;

  setCenter(c);
}

void
CSVGEllipse::
resizeTo(const CSize2D &size)
{
  setRadius(size.getWidth()/2.0, size.getHeight()/2.0);
}

void
CSVGEllipse::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<ellipse";

    CSVGObject::printValues(os);

    printNameValue(os, "cx", cx_);
    printNameValue(os, "cy", cy_);
    printNameValue(os, "rx", rx_);
    printNameValue(os, "ry", ry_);

    os << "/>" << std::endl;
  }
  else
    os << "ellipse " << getCenter() << " radius " << getRadiusX() << " " << getRadiusY();
}

std::ostream &
operator<<(std::ostream &os, const CSVGEllipse &ellipse)
{
  ellipse.print(os, false);

  return os;
}
