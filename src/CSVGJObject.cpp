#include <CSVGJObject.h>
#include <CSVGJTransform.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CSVGJObjectType::
CSVGJObjectType() :
 CJObjectType(CJToken::Type::Object, "SVGObject")
{
}

CSVGJObject::
CSVGJObject(CSVGObject *obj) :
 CJObject(obj->getSVG().js()->objectType()), obj_(obj)
{
  CJavaScript *js = obj->getSVG().js();

  type_->addFunction(js, "setAttribute");
  type_->addFunction(js, "setAttributeNS");
  type_->addFunction(js, "appendChild");
}

CJValueP
CSVGJObject::
getProperty(const std::string &key) const
{
  CJavaScript *js = obj_->getSVG().js();

  if      (key == "className")
    return js->createStringValue(obj_->getObjName());
  else if (key == "id")
    return js->createStringValue(obj_->getId());
  else if (key == "transform")
    return CJValueP(new CSVGJTransformStack(&obj_->getSVG(), obj_));
  else
    return CJObject::getProperty(key);
}

void
CSVGJObject::
setProperty(const std::string &key, CJValueP value)
{
  if      (key == "textContent") {
    std::string text = value->toString();

    obj_->setText(text);
  }
  else
    CJObject::setProperty(key, value);
}

CJValueP
CSVGJObject::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "setAttribute") {
    if (values.size() == 3) {
      std::string name  = (values[1] ? values[1]->toString() : "");
      std::string value = (values[2] ? values[2]->toString() : "");

      obj_->processOption(name, value);

      obj_->getSVG().redraw();
    }

    return CJValueP();
  }
  else if (name == "setAttributeNS") {
    if (values.size() == 4) {
      std::string name  = (values[2] ? values[2]->toString() : "");
      std::string value = (values[3] ? values[3]->toString() : "");

      obj_->processOption(name, value);

      obj_->getSVG().redraw();
    }

    return CJValueP();
  }
  else if (name == "appendChild") {
    if (values.size() == 2) {
      CJValueP objVal = values[1];

      CSVGJObjectP svgObj;

      if (objVal && objVal->type() == CJValue::Type::Object) {
        CJObjectP obj = std::static_pointer_cast<CJObject>(objVal);

        if (obj->type()->name() == "SVGObject") {
          svgObj = std::static_pointer_cast<CSVGJObject>(obj);
        }
      }

      if (svgObj) {
        obj_->addChildObject(svgObj->obj());
      }
    }

    return CJValueP();
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
