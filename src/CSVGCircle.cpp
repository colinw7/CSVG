#include "CSVGI.h"

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
 CSVGObject(svg),
 center_   (0,0),
 radius_   (1)
{
}

CSVGCircle::
CSVGCircle(const CSVGCircle &circle) :
 CSVGObject(circle),
 center_   (circle.center_),
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
processOption(const string &opt_name, const string &opt_value)
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

  double real;
  string str;

  if      (svg_.coordOption (opt_name, opt_value, "cx"       , &real))
    center_.x = real;
  else if (svg_.coordOption (opt_name, opt_value, "cy"       , &real))
    center_.y = real;
  else if (svg_.lengthOption(opt_name, opt_value, "r"        , &real))
    radius_ = real;
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

  svg_.drawCircle(center_.x, center_.y, radius_);
}

bool
CSVGCircle::
getBBox(CBBox2D &bbox) const
{
  if (! view_box_.isSet())
    bbox = CBBox2D(center_.x - radius_, center_.y - radius_,
                   center_.x + radius_, center_.y + radius_);
  else
    bbox = view_box_;

  return true;
}

void
CSVGCircle::
moveBy(const CVector2D &delta)
{
  center_ += delta;
}

void
CSVGCircle::
resizeTo(const CSize2D &size)
{
  radius_ = std::min(size.getWidth(), size.getHeight())/2.0;
}

void
CSVGCircle::
print(ostream &os) const
{
  os << "circle " << center_ << " radius " << radius_;
}

ostream &
operator<<(ostream &os, const CSVGCircle &circle)
{
  circle.print(os);

  return os;
}
