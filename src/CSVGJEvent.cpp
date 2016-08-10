#include <CSVGJEvent.h>
#include <CSVGJObject.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CSVGJEventType::
CSVGJEventType() :
 CJObjectType(CJToken::Type::Object, "SVGEvent")
{
}

CSVGJEvent::
CSVGJEvent(CSVG *svg) :
 CJObject(svg->js()->eventType()), svg_(svg)
{
  CJavaScript *js = svg->js();

  type_->addFunction(js, "getTarget");
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
