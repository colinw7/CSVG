#include <CSVGI.h>

CSVGLine::
CSVGLine(CSVG &svg) :
 CSVGObject(svg),
 point1_   (0,0),
 point2_   (0,0)
{
}

CSVGLine::
CSVGLine(const CSVGLine &line) :
 CSVGObject(line),
 point1_   (line.point1_),
 point2_   (line.point2_)
{
}

CSVGLine *
CSVGLine::
dup() const
{
  return new CSVGLine(*this);
}

bool
CSVGLine::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processPaintOption          (opt_name, opt_value)) return true;
  if (processColorOption          (opt_name, opt_value)) return true;
  if (processOpacityOption        (opt_name, opt_value)) return true;
  if (processGraphicsOption       (opt_name, opt_value)) return true;
  if (processMarkerOption         (opt_name, opt_value)) return true;
  if (processClipOption           (opt_name, opt_value)) return true;
  if (processMaskOption           (opt_name, opt_value)) return true;
  if (processFilterOption         (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processCursorOption         (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  std::string str;
  double      real;

  if      (svg_.coordOption(opt_name, opt_value, "x1", &real))
    point1_.x = real;
  else if (svg_.coordOption(opt_name, opt_value, "y1", &real))
    point1_.y = real;
  else if (svg_.coordOption(opt_name, opt_value, "x2", &real))
    point2_.x = real;
  else if (svg_.coordOption(opt_name, opt_value, "y2", &real))
    point2_.y = real;
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
CSVGLine::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.pathInit();

  svg_.pathMoveTo(point1_.x, point1_.y);
  svg_.pathLineTo(point2_.x, point2_.y);

  if (svg_.isFilled())
    svg_.pathFill();

  if (svg_.isStroked())
    svg_.pathStroke();
}

bool
CSVGLine::
getBBox(CBBox2D &bbox) const
{
  if (! view_box_.isSet())
    bbox = CBBox2D(point1_, point2_);
  else
    bbox = view_box_;

  return true;
}

void
CSVGLine::
moveBy(const CVector2D &delta)
{
  point1_ += delta;
  point2_ += delta;
}

void
CSVGLine::
resizeTo(const CSize2D &size)
{
  point2_.x = point1_.x + size.width ;
  point2_.y = point1_.y + size.height;
}

void
CSVGLine::
print(std::ostream &os) const
{
  os << "line " << point1_<< " " << point2_;
}

std::ostream &
operator<<(std::ostream &os, const CSVGLine &line)
{
  line.print(os);

  return os;
}
