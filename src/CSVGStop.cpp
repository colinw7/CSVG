#include <CSVGI.h>

CSVGStop::
CSVGStop(CSVG &svg) :
 CSVGObject(svg),
 offset_   (0),
 color_    (1,1,1),
 opacity_  (1)
{
}

CSVGStop::
CSVGStop(const CSVGStop &stop) :
 CSVGObject(stop),
 offset_   (stop.offset_),
 color_    (stop.color_),
 opacity_  (stop.opacity_)
{
}

CSVGStop *
CSVGStop::
dup() const
{
  return new CSVGStop(*this);
}

CRGBA
CSVGStop::
getAlphaColor() const
{
  CRGBA rgba = color_;

  rgba.setAlpha(opacity_);

  return rgba;
}

bool
CSVGStop::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

  if      (svg_.stringOption(opt_name, opt_value, "offset", str)) {
    if (! svg_.decodePercentString(str, &real))
      return false;

    offset_ = std::min(std::max(real, 0.0), 1.0);
  }
  else if (svg_.stringOption(opt_name, opt_value, "stop-color", str)) {
    CRGBA rgba;

    if (! svg_.decodeColorString(str, rgba))
      return false;

    color_ = rgba;
  }
  else if (svg_.realOption(opt_name, opt_value, "stop-opacity", &real)) {
    opacity_ = real;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGStop::
draw()
{
}

void
CSVGStop::
print(std::ostream &os) const
{
  os << "stop " << offset_;
}

std::ostream &
operator<<(std::ostream &os, const CSVGStop &stop)
{
  stop.print(os);

  return os;
}
