#include <CSVGStyleData.h>
#include <CSVG.h>

void
CSVGStyleData::
setValue(const std::string &name, const std::string &value)
{
  if      (name == "fill")
    fill_.setColor(value);
  else if (name == "fill-opacity")
    fill_.setOpacity(value);
  else if (name == "stroke")
    stroke_.setColor(value);
  else if (name == "stroke-width")
    stroke_.setWidth(value);
  else if (name == "stroke-dasharray")
    stroke_.setDash(value);
  else
    std::cerr << "Unhandled: " << name << ":" << value << std::endl;
}
