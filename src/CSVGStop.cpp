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
  double       real;
  CScreenUnits length;
  CSVGColor    color;
  bool         inherit;

  if      (svg_.percentOption(opt_name, opt_value, "offset", length)) {
    if (length.ratioValue() < 0.0 || length.ratioValue() > 1)
      length = CScreenUnits(std::min(std::max(length.ratioValue(), 0.0), 1.0));

    offset_ = length;
  }
  else if (svg_.colorOption(opt_name, opt_value, "stop-color", color, inherit)) {
    color_ = (! inherit ? Color(color) : Color::inherit());
  }
  else if (svg_.opacityOption(opt_name, opt_value, "stop-opacity", real, inherit)) {
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

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "stop " << getOffset();
}

void
CSVGStop::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNamePercent(os, "offset", offset_);

  if (color_.isValid())
    os << " stop-color=\"" << color_.getValue() << "\"";

  printNameValue(os, "stop-opacity", opacity_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGStop &stop)
{
  stop.print(os, false);

  return os;
}
