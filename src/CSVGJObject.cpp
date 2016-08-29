#include <CSVGJObject.h>
#include <CSVGJTransform.h>
#include <CSVGJAnimated.h>
#include <CSVGJavaScript.h>
#include <CSVGAnimateBase.h>
#include <CSVGCircle.h>
#include <CSVGMarker.h>
#include <CSVGRect.h>
#include <CSVGText.h>
#include <CSVGTSpan.h>
#include <CSVG.h>

CSVGJObjectType::
CSVGJObjectType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGObject")
{
}

CSVGJObject::
CSVGJObject(CSVGObject *obj) :
 CJObj(obj->getSVG().js(), obj->getSVG().js()->objectType()), obj_(obj)
{
  CJavaScript *js = obj->getSVG().js();

  type_->addObjectFunction(js, "setAttribute");
  type_->addObjectFunction(js, "setAttributeNS");
  type_->addObjectFunction(js, "appendChild");
  type_->addObjectFunction(js, "getTotalLength");
  type_->addObjectFunction(js, "getPointAtLength");
  type_->addObjectFunction(js, "getPathSegAtLength");
  type_->addObjectFunction(js, "getStartTime");
  type_->addObjectFunction(js, "getBBox");
}

CJValueP
CSVGJObject::
getProperty(CJavaScript *js, const std::string &key) const
{
  CSVG *svg = &obj_->getSVG();

  if      (key == "className")
    return js->createStringValue(obj_->getObjName());
  else if (key == "id")
    return js->createStringValue(obj_->getId());
  else if (key == "textContent")
    return js->createStringValue(obj_->getText());
  else if (key == "firstChild") {
    CSVGText *text = dynamic_cast<CSVGText *>(obj_);

    if (text && obj_->numChildren() == 0) {
      CSVGTSpan *tspan = svg->createTSpan();

      tspan->setText(text->getText());

      obj_->addChildObject(tspan);
    }

    if (obj_->numChildren() > 0)
      return CJValueP(new CSVGJObject(obj_->children().front()));
    else
      return CJValueP();
  }
  else if (key == "data") {
    return data_;
  }
  else if (key == "viewBox") {
    return CJValueP(new CSVGJAnimatedRect(svg, obj_, key));
  }
  else if (key == "preserveAspectRatio") {
    return CJValueP(new CSVGJAnimatedPreserveAspectRatio(svg, obj_, key));
  }
  else if (obj_->getObjTypeId() == CSVGObjTypeId::TEXT) {
    CSVGText *text = dynamic_cast<CSVGText *>(obj_);

    if      (key == "x" || key == "y" || key == "dx" || key == "dy")
      return CJValueP(new CSVGJAnimatedNumber(svg, text, key));
    else if (key == "rotate")
      return CJValueP(new CSVGJAnimatedNumberList(svg, text, "rotate"));
    else
      return CJObj::getProperty(js, key);
  }
  else if (obj_->getObjTypeId() == CSVGObjTypeId::CIRCLE) {
    CSVGCircle *circle = dynamic_cast<CSVGCircle *>(obj_);

    if      (key == "cx" || key == "cy" || key == "r")
      return CJValueP(new CSVGJAnimatedNumber(svg, circle, key));
    else if (key == "transform")
      return CJValueP(new CSVGJAnimatedTransformList(svg, circle, key));
    else
      return CJObj::getProperty(js, key);
  }
  else if (obj_->getObjTypeId() == CSVGObjTypeId::MARKER) {
    CSVGMarker *marker = dynamic_cast<CSVGMarker *>(obj_);

    if (key == "orientAngle")
      return CJValueP(new CSVGJAnimatedNumber(svg, marker, key));
    else
      return CJObj::getProperty(js, key);
  }
  else if (key == "transform")
    return CJValueP(new CSVGJTransformStack(svg, obj_));
  else
    return CJObj::getProperty(js, key);
}

void
CSVGJObject::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
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
    CJObj::setProperty(js, key, value);
}

CJValueP
CSVGJObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  CSVG *svg = &obj_->getSVG();

  if      (name == "setAttribute") {
    if (values.size() == 3) {
      std::string name  = (values[1] ? values[1]->toString() : "");
      std::string value = (values[2] ? values[2]->toString() : "");

      obj_->processOption(name, value);

      svg->redraw();
    }

    return CJValueP();
  }
  else if (name == "setAttributeNS") {
    if (values.size() == 4) {
      std::string name  = (values[2] ? values[2]->toString() : "");
      std::string value = (values[3] ? values[3]->toString() : "");

      obj_->processOption(name, value);

      svg->redraw();
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
  else if (name == "getBBox") {
    CBBox2D bbox;

    if (! obj_->getBBox(bbox))
      return CJValueP();

    return CJValueP(new CSVGJRect(svg, bbox));
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
