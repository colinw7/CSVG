#include <CSVGJEvent.h>

#ifdef CSVG_JAVASCRIPT

#include <CSVGJElement.h>
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

  objType_->addObjectFunction(js, "getTarget");
}

CJValueP
CSVGJEvent::
execNameFn(CJavaScript *, const std::string &name, const Values &)
{
  if (name == "getTarget") {
    CSVGObject *obj = svg_->eventObject();

    auto jobj = new CSVGJElement(obj);

    return CJValueP(jobj);
  }
  else
    return CJValueP();
}

#endif
