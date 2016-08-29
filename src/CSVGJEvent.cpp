#include <CSVGJEvent.h>
#include <CSVGJObject.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CSVGJEventType::
CSVGJEventType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGEvent")
{
}

//------

CSVGJEvent::
CSVGJEvent(CSVG *svg) :
 CJObj(svg->js(), svg->js()->eventType()), svg_(svg)
{
  CJavaScript *js = svg->js();

  type_->addObjectFunction(js, "getTarget");
}

CJValueP
CSVGJEvent::
execNameFn(CJavaScript *, const std::string &name, const Values &)
{
  if (name == "getTarget") {
    CSVGObject *obj = svg_->eventObject();

    auto jobj = new CSVGJObject(obj);

    return CJValueP(jobj);
  }
  else
    return CJValueP();
}
