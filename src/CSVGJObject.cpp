#include <CSVGJObject.h>
#include <CSVGJTransform.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CSVGJObjectType::
CSVGJObjectType() :
 CJObjectType(CJToken::Type::Object, "SVGObject")
{
  addFunction("className");
  addFunction("id");
  addFunction("setAttribute");
  addFunction("transform");
}

CSVGJObject::
CSVGJObject(CSVGObject *obj) :
 CJObject(obj->getSVG().js()->objectType()), obj_(obj)
{
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
    return CJValueP(new CSVGJTransformStack(&obj_->getSVG(), obj_));
  }
  else
    return CJValueP();
}

std::string
CSVGJObject::
toString() const
{
  std::ostringstream ss; ss << *this;
  return ss.str();
}

void
CSVGJObject::
print(std::ostream &os) const
{
  os << "SVGObject: " << *obj_;
}
