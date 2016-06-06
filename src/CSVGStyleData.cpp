#include <CSVGStyleData.h>
#include <CSVG.h>
#include <CSVGLog.h>

void
CSVGStyleData::
setValue(const std::string &name, const std::string &value)
{
  nameValues_[name] = value;

  if      (name == "fill")
    fill_.setColor(value);
  else if (name == "fill-opacity")
    fill_.setOpacity(value);
  else if (name == "stroke")
    stroke_.setColor(value);
  else if (name == "stroke-width")
    stroke_.setWidth(value);
  else if (name == "stroke-dasharray")
    stroke_.setDashArray(value);
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
  else if (name == "marker") {
    std::string  id;
    CSVGObject  *obj;

    if (svg_.decodeUrlObject(value, id, &obj)) {
      marker_.setStart(obj);
      marker_.setMid  (obj);
      marker_.setEnd  (obj);
    }
    else
      CSVGLog() << "Illegal url value '" << id << "' for " << name;
  }
  else
    CSVGLog() << "Unhandled style name: " << name << ":" << value;
}
