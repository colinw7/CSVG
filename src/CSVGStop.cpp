#include <CSVGStop.h>
#include <CSVG.h>

CSVGStop::
CSVGStop(CSVG &svg) :
 CSVGObject(svg)
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
  CRGBA rgba = getColor();

  if (opacity_.isValid())
    rgba.setAlpha(opacity_.getValue());

  return rgba;
}

bool
CSVGStop::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string  str;
  double       real;
  CScreenUnits length;

  if      (svg_.percentOption(opt_name, opt_value, "offset", length)) {
    if (length.px().value() < 0.0 || length.px().value() > 1)
      length = CScreenUnits(std::min(std::max(length.px().value(), 0.0), 1.0));

    offset_ = length;
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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<stop";

    CSVGObject::printValues(os);

    if (offset_.isValid()) {
      os << " offset=\""; printLength(os, offset_.getValue()); os << "\"";
    }

    if (color_.isValid())
      os << " stop-color=\"" << color_.getValue().getRGB().stringEncode() << "\"";

    if (opacity_.isValid())
      os << " stop-opacity=\"" << opacity_.getValue() << "\"";

    os << "/>" << std::endl;
  }
  else
    os << "stop " << getOffset();
}

std::ostream &
operator<<(std::ostream &os, const CSVGStop &stop)
{
  stop.print(os, false);

  return os;
}
