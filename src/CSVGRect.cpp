#include <CSVGRect.h>
#include <CSVGBuffer.h>
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
 CSVGObject(svg)
{
  updateBBox();
}

CSVGRect::
CSVGRect(const CSVGRect &rect) :
 CSVGObject(rect),
 x_        (rect.x_),
 y_        (rect.y_),
 width_    (rect.width_),
 height_   (rect.height_),
 rx_       (rect.rx_),
 ry_       (rect.ry_),
 bbox_     (rect.bbox_)
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
  double         real;
  CScreenUnits   length;
  CMatrixStack2D transform;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_ = length.px().value();
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length.px().value();
  else if (svg_.lengthOption(opt_name, opt_value, "rx", length))
    rx_ = length.px().value();
  else if (svg_.lengthOption(opt_name, opt_value, "ry", length))
    ry_ = length.px().value();
  else if (svg_.transformOption(opt_name, opt_value, "transform", transform))
    setTransform(transform);
  else
    return false;

  updateBBox();

  return true;
}

void
CSVGRect::
updateBBox()
{
  CPoint2D p1(getX()             , getY());
  CPoint2D p2(getX() + getWidth(), getY() + getHeight());

  bbox_ = CBBox2D(p1, p2);
}

void
CSVGRect::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  CSVGBuffer *buffer = svg_.getBuffer();

  double rx = getRX();
  double ry = getRY();

  if (rx > 0 || ry > 0) {
    if (rx <= 0) rx = ry;
    if (ry <= 0) ry = rx;

    if (svg_.isFilled() || svg_.isStroked()) {
      if (svg_.isFilled())
        buffer->fillRoundedRectangle(bbox_, rx, ry);

      if (svg_.isStroked())
        buffer->drawRoundedRectangle(bbox_, rx, ry);
    }
    else
      buffer->fillRoundedRectangle(bbox_, rx, ry);
  }
  else {
    if (svg_.isFilled() || svg_.isStroked()) {
      if (svg_.isFilled())
        buffer->fillRectangle(bbox_);

      if (svg_.isStroked())
        buffer->drawRectangle(bbox_);
    }
    else {
      buffer->fillRectangle(bbox_);
    }
  }
}

bool
CSVGRect::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid())
    bbox = bbox_;
  else
    bbox = getViewBox();

  return true;
}

void
CSVGRect::
setOrigin(const CPoint2D &point)
{
  x_ = point.x;
  y_ = point.y;

  updateBBox();
}

void
CSVGRect::
setSize(const CSize2D &size)
{
  width_  = size.width ;
  height_ = size.height;

  updateBBox();
}

void
CSVGRect::
moveTo(const CPoint2D &p)
{
  x_ = p.x;
  y_ = p.y;

  updateBBox();
}

void
CSVGRect::
moveBy(const CVector2D &delta)
{
  x_ = getX() + delta.x();
  y_ = getY() + delta.y();

  updateBBox();
}

void
CSVGRect::
resizeTo(const CSize2D &size)
{
  width_  = size.width ;
  height_ = size.height;

  updateBBox();
}

void
CSVGRect::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<rect";

    printNameValue(os, "x", x_);
    printNameValue(os, "y", y_);

    printNameValue(os, "width" , width_ );
    printNameValue(os, "height", height_);

    printNameValue(os, "rx", rx_);
    printNameValue(os, "ry", ry_);

    CSVGObject::printValues(os);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</rect>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "rect " << bbox_;
}

std::ostream &
operator<<(std::ostream &os, const CSVGRect &rect)
{
  rect.print(os, false);

  return os;
}
