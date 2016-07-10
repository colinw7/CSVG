#include <CSVGJEvent.h>
#include <CSVGJObject.h>
#include <CSVG.h>
#include <CSVGObject.h>

CSVGJEvent::
CSVGJEvent(CSVG *svg) :
 CJObject(svg->jsEventType()), svg_(svg)
{
  type_->addFunction("getTarget");
}

CJValueP
CSVGJEvent::
execNameFn(CJavaScript *, const std::string &name, const Values &)
{
  if (name == "getTarget") {
    CSVGObject *obj = svg_->eventObject();

    CSVGJObject *jobj = new CSVGJObject(obj);

    return CJValueP(jobj);
  }
  else
    return CJValueP();
}
