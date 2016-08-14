#include <CSVGJObject.h>
#include <CSVGJTransform.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGTSpan.h>
#include <CSVGRect.h>
#include <CSVGText.h>
#include <CSVGAnimateBase.h>
#include <CSVGJavaScript.h>

CSVGJObjectType::
CSVGJObjectType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGObject")
{
}

CSVGJObject::
CSVGJObject(CSVGObject *obj) :
 CJObj(obj->getSVG().js()->objectType()), obj_(obj)
{
  CJavaScript *js = obj->getSVG().js();

  type_->addObjectFunction(js, "setAttribute");
  type_->addObjectFunction(js, "setAttributeNS");
  type_->addObjectFunction(js, "appendChild");
  type_->addObjectFunction(js, "getTotalLength");
  type_->addObjectFunction(js, "getPointAtLength");
  type_->addObjectFunction(js, "getPathSegAtLength");
  type_->addObjectFunction(js, "getStartTime");
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
  else if (key == "textContent")
    return js->createStringValue(obj_->getText());
  else if (key == "firstChild") {
    CSVGText *text = dynamic_cast<CSVGText *>(obj_);

    if (text && obj_->numChildren() == 0) {
      CSVGTSpan *tspan = obj_->getSVG().createTSpan();

      tspan->setText(text->getText());

      obj_->addChildObject(tspan);
    }

    if (obj_->numChildren() > 0)
      return CJValueP(new CSVGJObject(obj_->children().front()));
    else
      return CJValueP();
  }
  else if (key == "data")
    return data_;
  else
    return CJObj::getProperty(key);
}

void
CSVGJObject::
setProperty(const std::string &key, CJValueP value)
{
  if      (key == "textContent") {
    std::string text = value->toString();

    obj_->setText(text);
  }
  else if (key == "data") {
    CSVGTSpan *tspan = dynamic_cast<CSVGTSpan *>(obj_);

    if (tspan)
      tspan->setText(value->toString());
    else
      data_ = value;
  }
  else
    CJObj::setProperty(key, value);
}

CJValueP
CSVGJObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
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
        CJObjP obj = std::static_pointer_cast<CJObj>(objVal);

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
  else if (name == "getTotalLength") {
    double len = obj_->pathLength();

    return js->createNumberValue(len);
  }
  else if (name == "getPointAtLength") {
    double l = 0;

    if (values.size() >= 2)
      l = values[1]->toReal();

    CPoint2D p;
    double   a;
    int      pi;

    (void) obj_->pointAtLength(l, p, a, pi);

    //---

    CJDictionary *dict = new CJDictionary(js);

    dict->setRealProperty(js, "x", p.x);
    dict->setRealProperty(js, "y", p.y);

    return CJValueP(dict);
  }
  else if (name == "getPathSegAtLength") {
    double l = 0;

    if (values.size() >= 2)
      l = values[1]->toReal();

    CPoint2D p;
    double   a;
    int      pi;

    (void) obj_->pointAtLength(l, p, a, pi);

    return js->createNumberValue(long(pi));
  }
  else if (name == "getStartTime") {
    CSVGAnimateBase *base = dynamic_cast<CSVGAnimateBase *>(obj_);

    double t = 0.0;

    if (base)
      t = base->getStartTime();

    return js->createNumberValue(t);
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
