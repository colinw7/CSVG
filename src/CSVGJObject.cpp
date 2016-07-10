#include <CSVGJObject.h>
#include <CSVGJTransform.h>
#include <CSVG.h>
#include <CSVGObject.h>

CSVGJObject::
CSVGJObject(CSVGObject *obj) :
 CJObject(obj->getSVG().jsObjectType()), obj_(obj)
{
  type_->addFunction("className");
  type_->addFunction("id");
  type_->addFunction("setAttribute");
  type_->addFunction("transform");
}

CJValueP
CSVGJObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if     (name == "className") {
    return js->createStringValue(obj_->getObjName());
  }
  else if (name == "id") {
    return js->createStringValue(obj_->getId());
  }
  else if (name == "setAttribute") {
    if (values.size() == 3) {
      std::string name  = values[1]->toString();
      std::string value = values[2]->toString();

      obj_->processOption(name, value);

      obj_->getSVG().redraw();
    }

    return CJValueP();
  }
  else if (name == "transform") {
    return CJValueP(new CSVGJTransformStack(&obj_->getSVG(), obj_->getTransform()));
  }
  else
    return CJValueP();
}
