#include <CSVGI.h>

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
    x <Coordinate>
    y <Coordinate>
    width <Length>
    height <Length>
    rx <Length>
    ry <Length>
    transform <TransformList>
*/

CSVGRect::
CSVGRect(CSVG &svg) :
 CSVGObject(svg),
 bbox_     (),
 x_        (0),
 y_        (0),
 width_    (1),
 height_   (1),
 rx_       (0),
 ry_       (0)
{
  bbox_ = CBBox2D(CPoint2D(x_, y_), CPoint2D(x_ + width_, y_ + height_));
}

CSVGRect::
CSVGRect(const CSVGRect &rect) :
 CSVGObject(rect),
 bbox_     (rect.bbox_),
 x_        (rect.x_),
 y_        (rect.y_),
 width_    (rect.width_),
 height_   (rect.height_),
 rx_       (rect.rx_),
 ry_       (rect.ry_)
{
}

CSVGRect *
CSVGRect::
dup() const
{
  return new CSVGRect(*this);
}

bool
CSVGRect::
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

  string str;
  double real;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", &real))
    width_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "height", &real))
    height_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "rx", &real))
    rx_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "ry", &real))
    ry_ = real;
  else if (svg_.stringOption(opt_name, opt_value, "transform", str)) {
    CMatrix2D transform;

    if (! svg_.decodeTransform(str, transform))
      return false;

    setTransform(transform);
  }
  else
    return false;

  bbox_ = CBBox2D(CPoint2D(x_, y_), CPoint2D(x_ + width_, y_ + height_));

  return true;
}

void
CSVGRect::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  if (rx_ > 0 || ry_ > 0) {
    if (rx_ <= 0) rx_ = ry_;
    if (ry_ <= 0) ry_ = rx_;

    if (svg_.isFilled())
      svg_.fillRoundedRectangle(bbox_, rx_, ry_);

    if (svg_.isStroked())
      svg_.drawRoundedRectangle(bbox_, rx_, ry_);
  }
  else {
    if (svg_.isFilled())
      svg_.fillRectangle(bbox_);

    if (svg_.isStroked())
      svg_.drawRectangle(bbox_);
  }
}

bool
CSVGRect::
getBBox(CBBox2D &bbox) const
{
  if (! view_box_.isSet())
    bbox = bbox_;
  else
    bbox = view_box_;

  return true;
}

void
CSVGRect::
moveBy(const CVector2D &delta)
{
  bbox_.moveBy(delta);
}

void
CSVGRect::
resizeTo(const CSize2D &size)
{
  bbox_.setSize(size);
}

void
CSVGRect::
print(ostream &os) const
{
  os << "rect " << bbox_;
}

ostream &
operator<<(ostream &os, const CSVGRect &rect)
{
  rect.print(os);

  return os;
}
