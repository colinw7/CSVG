#include <CSVGJAnimated.h>
#include <CSVGJObject.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CJObjTypeP CSVGJAnimatedIntegerType::type_;

CJObjTypeP
CSVGJAnimatedIntegerType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedIntegerType(js));

  return type_;
}

CSVGJAnimatedIntegerType::
CSVGJAnimatedIntegerType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedInteger")
{
}

//------

CSVGJAnimatedInteger::
CSVGJAnimatedInteger(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAnimatedIntegerType::instance(svg->js())),
 svg_(svg), obj_(obj), name_(name)
{
}

CJValueP
CSVGJAnimatedInteger::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedInteger::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }

  return CJObj::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedNumberListType::type_;

CJObjTypeP
CSVGJAnimatedNumberListType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedNumberListType(js));

  return type_;
}

CSVGJAnimatedNumberListType::
CSVGJAnimatedNumberListType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedNumberList")
{
}

//------

CSVGJAnimatedNumberList::
CSVGJAnimatedNumberList(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAnimatedNumberListType::instance(svg->js())),
 svg_(svg), obj_(obj), name_(name)
{
}

CJValueP
CSVGJAnimatedNumberList::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedNumberList::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }

  return CJObj::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedNumberType::type_;

CJObjTypeP
CSVGJAnimatedNumberType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedNumberType(js));

  return type_;
}

CSVGJAnimatedNumberType::
CSVGJAnimatedNumberType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedNumber")
{
}

//------

CSVGJAnimatedNumber::
CSVGJAnimatedNumber(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAnimatedNumberType::instance(svg->js())),
 svg_(svg), obj_(obj), name_(name)
{
}

CJValueP
CSVGJAnimatedNumber::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedNumber::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "animVal") {
    COptReal real = obj_->getRealNameValue(name_);

    return js->createNumberValue(real.getValue(0));
  }
  else if (name == "baseVal") {
    COptReal real = obj_->getRealNameValue(name_);

    return js->createNumberValue(real.getValue(0));
  }

  return CJObj::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedLengthListType::type_;

CJObjTypeP
CSVGJAnimatedLengthListType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedLengthListType(js));

  return type_;
}

CSVGJAnimatedLengthListType::
CSVGJAnimatedLengthListType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedLengthList")
{
}

//------

CSVGJAnimatedLengthList::
CSVGJAnimatedLengthList(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAnimatedLengthListType::instance(svg->js())),
 svg_(svg), obj_(obj), name_(name)
{
}

CJValueP
CSVGJAnimatedLengthList::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedLengthList::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }

  return CJObj::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedLengthType::type_;

CJObjTypeP
CSVGJAnimatedLengthType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedLengthType(js));

  return type_;
}

CSVGJAnimatedLengthType::
CSVGJAnimatedLengthType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedLength")
{
}

//------

CSVGJAnimatedLength::
CSVGJAnimatedLength(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAnimatedLengthType::instance(svg->js())),
 svg_(svg), obj_(obj), name_(name)
{
}

CJValueP
CSVGJAnimatedLength::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedLength::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "animVal") {
    COptReal real = obj_->getRealNameValue(name_);

    return js->createNumberValue(real.getValue(0));
  }
  else if (name == "baseVal") {
    COptReal real = obj_->getRealNameValue(name_);

    return js->createNumberValue(real.getValue(0));
  }

  return CJObj::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedRectType::type_;

CJObjTypeP
CSVGJAnimatedRectType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedRectType(js));

  return type_;
}

CSVGJAnimatedRectType::
CSVGJAnimatedRectType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedRect")
{
}

//------

CSVGJAnimatedRect::
CSVGJAnimatedRect(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAnimatedRectType::instance(svg->js())),
 svg_(svg), obj_(obj), name_(name)
{
}

CJValueP
CSVGJAnimatedRect::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedRect::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }

  return CJObj::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedTransformListType::type_;

CJObjTypeP
CSVGJAnimatedTransformListType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedTransformListType(js));

  return type_;
}

CSVGJAnimatedTransformListType::
CSVGJAnimatedTransformListType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedTransformList")
{
}

//------

CSVGJAnimatedTransformList::
CSVGJAnimatedTransformList(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAnimatedTransformListType::instance(svg->js())),
 svg_(svg), obj_(obj), name_(name)
{
}

CJValueP
CSVGJAnimatedTransformList::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedTransformList::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }

  return CJObj::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedPreserveAspectRatioType::type_;

CJObjTypeP
CSVGJAnimatedPreserveAspectRatioType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedPreserveAspectRatioType(js));

  return type_;
}

CSVGJAnimatedPreserveAspectRatioType::
CSVGJAnimatedPreserveAspectRatioType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedPreserveAspectRatio")
{
}

//------

CSVGJAnimatedPreserveAspectRatio::
CSVGJAnimatedPreserveAspectRatio(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAnimatedPreserveAspectRatioType::instance(svg->js())),
 svg_(svg), obj_(obj), name_(name)
{
}

CJValueP
CSVGJAnimatedPreserveAspectRatio::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedPreserveAspectRatio::
getProperty(CJavaScript *js, const std::string &name) const
{
  COptString str;

  CSVGPreserveAspect preserveAspect;

  if (obj_->getObjTypeId() == CSVGObjTypeId::BLOCK) {
    CSVGBlock *block = dynamic_cast<CSVGBlock *>(obj_);

    preserveAspect = block->preserveAspect();

    str = preserveAspect.toString();
  }

  if      (name == "animVal") {
    return js->createStringValue(str.getValue(""));
  }
  else if (name == "baseVal") {
    return js->createStringValue(str.getValue(""));
  }

  return CJObj::getProperty(js, name);
}
