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
    rx        <Length>
    ry        <Length>
    transform <TransformList>
*/

CSVGEllipse::
CSVGEllipse(CSVG &svg) :
 CSVGObject(svg),
 center_   (0,0),
 rx_       (1),
 ry_       (1)
{
}

CSVGEllipse::
CSVGEllipse(const CSVGEllipse &ellipse) :
 CSVGObject(ellipse),
 center_   (ellipse.center_),
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

  if      (svg_.coordOption (opt_name, opt_value, "cx", &real))
    center_.x = real;
  else if (svg_.coordOption (opt_name, opt_value, "cy", &real))
    center_.y = real;
  else if (svg_.lengthOption(opt_name, opt_value, "rx" , &real))
    rx_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "ry" , &real))
    ry_ = real;
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
CSVGEllipse::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.drawEllipse(center_.x, center_.y, rx_, ry_);
}

bool
CSVGEllipse::
getBBox(CBBox2D &bbox) const
{
  if (! view_box_.isSet())
    bbox = CBBox2D(center_.x - rx_, center_.y - ry_,
                   center_.x + rx_, center_.y + ry_);
  else
    bbox = view_box_;

  return true;
}

void
CSVGEllipse::
moveBy(const CVector2D &delta)
{
  center_ += delta;
}

void
CSVGEllipse::
resizeTo(const CSize2D &size)
{
  rx_ = size.getWidth ()/2.0;
  ry_ = size.getHeight()/2.0;
}

void
CSVGEllipse::
print(ostream &os) const
{
  os << "ellipse " << center_ << " radius " << rx_ << " " << ry_;
}

ostream &
operator<<(ostream &os, const CSVGEllipse &ellipse)
{
  ellipse.print(os);

  return os;
}
