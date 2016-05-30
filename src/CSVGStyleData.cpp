#include <CSVGStyleData.h>
#include <CSVG.h>
#include <CSVGLog.h>

void
CSVGStyleData::
setValue(const std::string &name, const std::string &value)
{
  if      (name == "fill") {
    if (value == "none")
      fill_.setNoColor(true);
    else
      fill_.setColor(value);
  }
  else if (name == "fill-opacity")
    fill_.setOpacity(value);
  else if (name == "stroke") {
    if (value == "none")
      stroke_.setNoColor(true);
    else
      stroke_.setColor(value);
  }
  else if (name == "stroke-width")
    stroke_.setWidth(value);
  else if (name == "stroke-dasharray")
    stroke_.setDash(value);
  else if (name == "marker-start") {
    std::string  id;
    CSVGObject  *obj;

    if (svg_.decodeUrlObject(value, id, &obj))
      marker_.setStart(obj);
    else
      CSVGLog() << "Illegal url value '" << id << "' for " << name;
  }
  else if (name == "marker-mid") {
    std::string  id;
    CSVGObject  *obj;

    if (svg_.decodeUrlObject(value, id, &obj))
      marker_.setMid(obj);
    else
      CSVGLog() << "Illegal url value '" << id << "' for " << name;
  }
  else if (name == "marker-end") {
    std::string  id;
    CSVGObject  *obj;

    if (svg_.decodeUrlObject(value, id, &obj))
      marker_.setEnd(obj);
    else
      CSVGLog() << "Illegal url value '" << id << "' for " << name;
  }
  else
    CSVGLog() << "Unhandled: " << name << ":" << value;
}
