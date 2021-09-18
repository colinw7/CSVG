#include <CSVGJElement.h>

#ifdef CSVG_JAVASCRIPT

#include <CSVGJTypes.h>
#include <CSVGJAnimated.h>
#include <CSVGJavaScript.h>
#include <CSVGAnimateBase.h>
#include <CSVGCircle.h>
#include <CSVGFeTurbulence.h>
#include <CSVGMarker.h>
#include <CSVGRect.h>
#include <CSVGText.h>
#include <CSVGTSpan.h>
#include <CSVG.h>

CSVGJElementType::
CSVGJElementType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGElement")
{
}

//------

/*
  interface SVGElement : Element {
             attribute DOMString id setraises(DOMException);
             attribute DOMString xmlbase setraises(DOMException);
    readonly attribute SVGSVGElement ownerSVGElement;
    readonly attribute SVGElement viewportElement;
};

readonly attribute
*/
CSVGJElement::
CSVGJElement(CSVGObject *obj) :
 CJObj(obj->getSVG().js(), obj->getSVG().js()->objectType()), obj_(obj)
{
  auto *js = obj->getSVG().js();

  objType_->addObjectFunction(js, "setAttribute");
  objType_->addObjectFunction(js, "getAttributeNS");
  objType_->addObjectFunction(js, "setAttributeNS");
  objType_->addObjectFunction(js, "appendChild");
  objType_->addObjectFunction(js, "getTotalLength");
  objType_->addObjectFunction(js, "getPointAtLength");
  objType_->addObjectFunction(js, "getPathSegAtLength");
  objType_->addObjectFunction(js, "getStartTime");
  objType_->addObjectFunction(js, "getBBox");
  objType_->addObjectFunction(js, "getCTM");
  objType_->addObjectFunction(js, "getScreenCTM");
  objType_->addObjectFunction(js, "getTransformToElement");
}

CJValueP
CSVGJElement::
getProperty(CJavaScript *js, const std::string &key) const
{
  auto *svg = &obj_->getSVG();

  if      (key == "className") {
    return CJValueP(new CSVGJAnimatedString(svg, obj_, key));
  }
  else if (key == "id") {
    return js->createStringValue(obj_->getId());
  }
  else if (key == "textContent") {
    return js->createStringValue(obj_->getText());
  }
  else if (key == "firstChild") {
    auto *text = dynamic_cast<CSVGText *>(obj_);

    if (text && obj_->numChildren() == 0) {
      auto *tspan = svg->createTSpan();

      tspan->setText(text->getText());

      obj_->addChildObject(tspan);
    }

    if (obj_->numChildren() > 0)
      return CJValueP(new CSVGJElement(obj_->children().front()));
    else
      return CJValueP();
  }
  else if (key == "farthestViewportElement") {
    CBBox2D bbox;

    obj_->getFlatTransformedBBox(bbox);

    CSVGObject* maxObj = nullptr;
    double      d      = 1E50;

    CSVG::ObjectList objects;

    svg->getAllChildren(objects);

    for (const auto &o : objects) {
      if (o == obj_) continue;

      CBBox2D bbox1;

      o->getFlatTransformedBBox(bbox1);

      double d1 = bbox.distanceTo(bbox1);

      if (! maxObj || d1 > d) {
        maxObj = o;
        d      = d1;
      }
    }

    if (maxObj)
      return CJValueP(new CSVGJElement(maxObj));
    else
      return CJValueP();
  }
  else if (key == "nearestViewportElement") {
    CBBox2D bbox;

    obj_->getFlatTransformedBBox(bbox);

    CSVGObject* minObj = nullptr;
    double      d      = 1E50;

    CSVG::ObjectList objects;

    svg->getAllChildren(objects);

    for (const auto &o : objects) {
      if (o == obj_) continue;

      CBBox2D bbox1;

      o->getFlatTransformedBBox(bbox1);

      double d1 = bbox.distanceTo(bbox1);

      if (! minObj || d1 < d) {
        minObj = o;
        d      = d1;
      }
    }

    if (minObj)
      return CJValueP(new CSVGJElement(minObj));
    else
      return CJValueP();
  }
  else if (key == "data") {
    return data_;
  }
  else if (key == "viewBox") {
    return CJValueP(new CSVGJAnimatedRect(svg, obj_, key));
  }
  else if (key == "transform") {
    return CJValueP(new CSVGJAnimatedTransformList(svg, obj_, key));
  }
  else if (key == "externalResourcesRequired") {
    return CJValueP(new CSVGJAnimatedBoolean(svg, obj_, key));
  }
  else if (obj_->getObjTypeId() == CSVGObjTypeId::BLOCK) {
    auto *block = dynamic_cast<CSVGBlock *>(obj_);

    if (key == "preserveAspectRatio")
      return CJValueP(new CSVGJAnimatedPreserveAspectRatio(svg, block, key));
    else
      return CJObj::getProperty(js, key);
  }
  else if (obj_->getObjTypeId() == CSVGObjTypeId::TEXT) {
    auto *text = dynamic_cast<CSVGText *>(obj_);

    if      (key == "x" || key == "y")
      return CJValueP(new CSVGJAnimatedLengthList(svg, text, key));
    else if (key == "dx" || key == "dy")
      return CJValueP(new CSVGJAnimatedLengthList(svg, text, key));
    else if (key == "rotate")
      return CJValueP(new CSVGJAnimatedNumberList(svg, text, key));
    else if (key == "lengthAdjust")
      return CJValueP(new CSVGJAnimatedEnumeration(svg, text, key));
    else
      return CJObj::getProperty(js, key);
  }
  else if (obj_->getObjTypeId() == CSVGObjTypeId::CIRCLE) {
    auto *circle = dynamic_cast<CSVGCircle *>(obj_);

    if      (key == "cx" || key == "cy")
      return CJValueP(new CSVGJAnimatedNumber(svg, circle, key));
    else if (key == "r")
      return CJValueP(new CSVGJAnimatedLength(svg, circle, key));
    else
      return CJObj::getProperty(js, key);
  }
  else if (obj_->getObjTypeId() == CSVGObjTypeId::MARKER) {
    auto *marker = dynamic_cast<CSVGMarker *>(obj_);

    if (key == "orientAngle")
      return CJValueP(new CSVGJAnimatedAngle(svg, marker, key));
    else
      return CJObj::getProperty(js, key);
  }
  else if (obj_->getObjTypeId() == CSVGObjTypeId::FE_TURBULENCE) {
    auto *feTurbulence = dynamic_cast<CSVGFeTurbulence *>(obj_);

    if      (key == "baseFrequencyX" || key == "baseFrequencyY" || key == "seed")
      return CJValueP(new CSVGJAnimatedNumber(svg, feTurbulence, key));
    else if (key == "numOctaves")
      return CJValueP(new CSVGJAnimatedInteger(svg, feTurbulence, key));
    else if (key == "stitchTiles")
      return CJValueP(new CSVGJAnimatedEnumeration(svg, feTurbulence, key));
    else if (key == "type")
      return CJValueP(new CSVGJAnimatedEnumeration(svg, feTurbulence, key));
    else
      return CJObj::getProperty(js, key);
  }
  else
    return CJObj::getProperty(js, key);
}

void
CSVGJElement::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if      (key == "textContent") {
    std::string text = value->toString();

    obj_->setText(text);
  }
  else if (key == "data") {
    auto *tspan = dynamic_cast<CSVGTSpan *>(obj_);

    if (tspan)
      tspan->setText(value->toString());
    else
      data_ = value;
  }
  else
    CJObj::setProperty(js, key, value);
}

CJValueP
CSVGJElement::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  auto *svg = &obj_->getSVG();

  if      (name == "setAttribute") {
    if (values.size() == 3) {
      std::string name1  = (values[1] ? values[1]->toString() : "");
      std::string value1 = (values[2] ? values[2]->toString() : "");

      obj_->processOption(name1, value1);

      svg->redraw();
    }

    return CJValueP();
  }
  else if (name == "getAttributeNS") {
    CJValueP value;

    if (values.size() == 3) {
      std::string name1 = (values[2] ? values[2]->toString() : "");

      value = getProperty(js, name1);
    }

    return value;
  }
  else if (name == "setAttributeNS") {
    if (values.size() == 4) {
      std::string name1  = (values[2] ? values[2]->toString() : "");
      std::string value1 = (values[3] ? values[3]->toString() : "");

      obj_->processOption(name1, value1);

      svg->redraw();
    }

    return CJValueP();
  }
  else if (name == "appendChild") {
    if (values.size() == 2) {
      CJValueP objVal = values[1];

      CSVGJElementP svgObj;

      if (objVal && objVal->type() == CJValue::Type::Object) {
        CJObjP obj = std::static_pointer_cast<CJObj>(objVal);

        if (obj->objType()->name() == "SVGElement") {
          svgObj = std::static_pointer_cast<CSVGJElement>(obj);
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
      l = values[1]->toReal().getValue(0);

    CPoint2D p;
    double   a;
    int      pi;

    (void) obj_->pointAtLength(l, p, a, pi);

    //---

    auto *dict = new CJDictionary(js, "");

    dict->setRealProperty(js, "x", p.x);
    dict->setRealProperty(js, "y", p.y);

    return CJValueP(dict);
  }
  else if (name == "getPathSegAtLength") {
    double l = 0;

    if (values.size() >= 2)
      l = values[1]->toReal().getValue(0);

    CPoint2D p;
    double   a;
    int      pi;

    (void) obj_->pointAtLength(l, p, a, pi);

    return js->createNumberValue(long(pi));
  }
  else if (name == "getStartTime") {
    auto *base = dynamic_cast<CSVGAnimateBase *>(obj_);

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
  else if (name == "getCTM") {
    auto matrix = obj_->getTransform().getMatrix();

    return CJValueP(new CSVGJMatrix(svg, matrix));
  }
  else if (name == "getScreenCTM") {
    auto matrix = obj_->getFlatTransform().getMatrix();

    return CJValueP(new CSVGJMatrix(svg, matrix));
  }
  else if (name == "getTransformToElement") {
    if (values.size() == 2) {
      CJValueP objVal = values[1];

      CSVGJElementP svgObj;

      if (objVal && objVal->type() == CJValue::Type::Object) {
        CJObjP obj = std::static_pointer_cast<CJObj>(objVal);

        if (obj->objType()->name() == "SVGElement") {
          svgObj = std::static_pointer_cast<CSVGJElement>(obj);
        }
      }

      if (svgObj) {
        auto matrixStack = obj_->getTransformTo(svgObj->obj());

        auto matrix = matrixStack.getMatrix();

        return CJValueP(new CSVGJMatrix(svg, matrix));
      }
    }

    return CJValueP();
  }
  else
    return CJValueP();
}

std::string
CSVGJElement::
toString() const
{
  std::ostringstream ss; ss << *this;
  return ss.str();
}

void
CSVGJElement::
print(std::ostream &os) const
{
  os << "SVGElement ";

  CJObj::print(os);
}

#endif
