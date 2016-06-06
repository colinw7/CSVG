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

bool
CSVGStop::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string  str;
  double       real;
  CScreenUnits length;

  if      (svg_.percentOption(opt_name, opt_value, "offset", length)) {
    if (length.ratioValue() < 0.0 || length.ratioValue() > 1)
      length = CScreenUnits(std::min(std::max(length.ratioValue(), 0.0), 1.0));

    offset_ = length;
  }
  else if (svg_.stringOption(opt_name, opt_value, "stop-color", str)) {
    CSVGColor color;

    if (! svg_.decodeColorString(str, color))
      return false;

    color_ = color;
  }
  else if (svg_.realOption(opt_name, opt_value, "stop-opacity", &real)) {
    opacity_ = real;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGStop::
draw()
{
  return false;
}

void
CSVGStop::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<stop";

    CSVGObject::printValues(os);

    printNameValue(os, "offset", offset_);

    if (color_.isValid())
      os << " stop-color=\"" << color_.getValue() << "\"";

    printNameValue(os, "stop-opacity=", opacity_);

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
