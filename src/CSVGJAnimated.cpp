#include <CSVGJAnimated.h>
#include <CSVGJElement.h>
#include <CSVGJTypes.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CSVGJObjValue::
CSVGJObjValue(CSVG *svg, CSVGObject *obj, const std::string &name, CJObjTypeP type) :
 CJObj(svg->js(), type), svg_(svg), obj_(obj), name_(name)
{
}

//------

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

/*
  interface SVGAnimatedInteger {
    attribute boolean baseVal setraises(DOMException);
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedInteger::
CSVGJAnimatedInteger(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedIntegerType::instance(svg->js()))
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
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }

  return CSVGJObjValue::getProperty(js, name);
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

/*
  interface CSVGJAnimatedNumberList {
    readonly attribute boolean baseVal;
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedNumberList::
CSVGJAnimatedNumberList(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedNumberListType::instance(svg->js()))
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
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    CSVGJNumberList::Reals reals;

    CSVG::stringToReals(str.getValue(""), reals);

    return CJValueP(new CSVGJNumberList(svg_, reals));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    CSVGJNumberList::Reals reals;

    CSVG::stringToReals(str.getValue(""), reals);

    return CJValueP(new CSVGJNumberList(svg_, reals));
  }

  return CSVGJObjValue::getProperty(js, name);
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

/*
  interface SVGAnimatedBoolean {
    attribute boolean baseVal setraises(DOMException);
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedNumber::
CSVGJAnimatedNumber(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedNumberType::instance(svg->js()))
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
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptReal real = obj_->getRealNameValue(name_);

    return CJValueP(new CSVGJNumber(svg_, real.getValue(0)));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptReal real = obj_->getRealNameValue(name_);

    return CJValueP(new CSVGJNumber(svg_, real.getValue(0)));
  }

  return CSVGJObjValue::getProperty(js, name);
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

/*
  interface SVGAnimatedLengthList {
    readonly attribute boolean baseVal;
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedLengthList::
CSVGJAnimatedLengthList(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedLengthListType::instance(svg->js()))
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
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    std::vector<CScreenUnits> lengths;

    CSVG::decodeLengthListValue(str.getValue(""), lengths);

    return CJValueP(new CSVGJLengthList(svg_, lengths));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    std::vector<CScreenUnits> lengths;

    CSVG::decodeLengthListValue(str.getValue(""), lengths);

    return CJValueP(new CSVGJLengthList(svg_, lengths));
  }

  return CSVGJObjValue::getProperty(js, name);
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

/*
  interface SVGAnimatedLength {
    readonly attribute boolean baseVal;
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedLength::
CSVGJAnimatedLength(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedLengthType::instance(svg->js()))
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
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    COptValT<CScreenUnits> length = CSVG::decodeLengthValue(str.getValue(""));

    return CJValueP(new CSVGJLength(svg_, length.getValue(CScreenUnits())));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    return CJValueP(new CSVGJLength(svg_, obj_, name_));
  }

  return CSVGJObjValue::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedStringType::type_;

CJObjTypeP
CSVGJAnimatedStringType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedStringType(js));

  return type_;
}

CSVGJAnimatedStringType::
CSVGJAnimatedStringType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedString")
{
}

//------

/*
  interface SVGAnimatedString {
    attribute boolean baseVal setraises(DOMException);
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedString::
CSVGJAnimatedString(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedStringType::instance(svg->js()))
{
}

CJValueP
CSVGJAnimatedString::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedString::
getProperty(CJavaScript *js, const std::string &name) const
{
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }

  return CSVGJObjValue::getProperty(js, name);
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

/*
  interface SVGAnimatedRect {
    readonly attribute boolean baseVal;
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedRect::
CSVGJAnimatedRect(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedRectType::instance(svg->js()))
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
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    CBBox2D bbox;

    CSVG::decodeBBoxString(str.getValue(""), bbox);

    return CJValueP(new CSVGJRect(svg_, bbox));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    CBBox2D bbox;

    CSVG::decodeBBoxString(str.getValue(""), bbox);

    return CJValueP(new CSVGJRect(svg_, bbox));
  }

  return CSVGJObjValue::getProperty(js, name);
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

/*
  interface SVGAnimatedTransformList {
    readonly attribute SVGTransformList baseVal;
    readonly attribute SVGTransformList animVal;
  };
*/
CSVGJAnimatedTransformList::
CSVGJAnimatedTransformList(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedTransformListType::instance(svg->js()))
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
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    return js->createStringValue(str.getValue(""));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    if (name_ == "transform") {
      return CJValueP(new CSVGJTransformList(svg_, obj_));
    }
    else {
      COptString str = obj_->getNameValue(name_);

      return js->createStringValue(str.getValue(""));
    }
  }

  return CSVGJObjValue::getProperty(js, name);
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

/*
  interface SVGAnimatedPreserveAspectRatio {
    readonly attribute SVGPreserveAspectRatio baseVal;
    readonly attribute SVGPreserveAspectRatio animVal;
  };
*/
CSVGJAnimatedPreserveAspectRatio::
CSVGJAnimatedPreserveAspectRatio(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedPreserveAspectRatioType::instance(svg->js()))
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
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    CSVGPreserveAspect preserveAspect;

    CSVG::decodePreserveAspectRatio(str.getValue(""), preserveAspect);

    return CJValueP(new CSVGJPreserveAspectRatio(svg_, preserveAspect));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    CSVGPreserveAspect preserveAspect;

    CSVG::decodePreserveAspectRatio(str.getValue(""), preserveAspect);

    return CJValueP(new CSVGJPreserveAspectRatio(svg_, preserveAspect));
  }

  return CSVGJObjValue::getProperty(js, name);
}

//------

CJObjTypeP CSVGJPreserveAspectRatioType::type_;

CJObjTypeP
CSVGJPreserveAspectRatioType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJPreserveAspectRatioType(js));

  return type_;
}

CSVGJPreserveAspectRatioType::
CSVGJPreserveAspectRatioType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGPreserveAspectRatio")
{
}

//------

/*
  interface SVGPreserveAspectRatio {
    // Alignment Types
    const unsigned short SVG_PRESERVEASPECTRATIO_UNKNOWN = 0;
    const unsigned short SVG_PRESERVEASPECTRATIO_NONE = 1;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMINYMIN = 2;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMIDYMIN = 3;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMAXYMIN = 4;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMINYMID = 5;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMIDYMID = 6;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMAXYMID = 7;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMINYMAX = 8;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMIDYMAX = 9;
    const unsigned short SVG_PRESERVEASPECTRATIO_XMAXYMAX = 10;

    // Meet-or-slice Types
    const unsigned short SVG_MEETORSLICE_UNKNOWN = 0;
    const unsigned short SVG_MEETORSLICE_MEET = 1;
    const unsigned short SVG_MEETORSLICE_SLICE = 2;

    attribute unsigned short align setraises(DOMException);
    attribute unsigned short meetOrSlice setraises(DOMException);
  };
*/
CSVGJPreserveAspectRatio::
CSVGJPreserveAspectRatio(CSVG *svg, const CSVGPreserveAspect &preserveAspect) :
 CJObj(svg->js(), CSVGJPreserveAspectRatioType::instance(svg->js())),
 svg_(svg), preserveAspect_(preserveAspect)
{
}

CJValueP
CSVGJPreserveAspectRatio::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJPreserveAspectRatio::
getProperty(CJavaScript *js, const std::string &name) const
{
  COptString str;

  if      (name == "align") {
    return js->createNumberValue(long(preserveAspect_.alignValue()));
  }
  else if (name == "meetOrSlice") {
    return js->createNumberValue(long(preserveAspect_.meetOrSliceValue()));
  }

  return CJObj::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedEnumerationType::type_;

CJObjTypeP
CSVGJAnimatedEnumerationType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedEnumerationType(js));

  return type_;
}

CSVGJAnimatedEnumerationType::
CSVGJAnimatedEnumerationType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedEnumeration")
{
}

//------

/*
  interface SVGAnimatedEnumeration {
    attribute boolean baseVal setraises(DOMException);
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedEnumeration::
CSVGJAnimatedEnumeration(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedEnumerationType::instance(svg->js()))
{
}

CJValueP
CSVGJAnimatedEnumeration::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedEnumeration::
getProperty(CJavaScript *js, const std::string &name) const
{
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptInt value = obj_->getIntegerNameValue(name_);

    return js->createNumberValue(long(value.getValue(0)));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptInt value = obj_->getIntegerNameValue(name_);

    return js->createNumberValue(long(value.getValue(0)));
  }

  return CSVGJObjValue::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedBooleanType::type_;

CJObjTypeP
CSVGJAnimatedBooleanType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedBooleanType(js));

  return type_;
}

CSVGJAnimatedBooleanType::
CSVGJAnimatedBooleanType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedBoolean")
{
}

//------

/*
  interface SVGAnimatedBoolean {
    attribute boolean baseVal setraises(DOMException);
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedBoolean::
CSVGJAnimatedBoolean(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedBooleanType::instance(svg->js()))
{
}

CJValueP
CSVGJAnimatedBoolean::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedBoolean::
getProperty(CJavaScript *js, const std::string &name) const
{
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    bool b;

    CSVG::decodeBoolean(str.getValue(""), b);

    return js->createBoolValue(b);
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    COptString str = obj_->getNameValue(name_);

    bool b;

    CSVG::decodeBoolean(str.getValue(""), b);

    return js->createBoolValue(b);
  }

  return CSVGJObjValue::getProperty(js, name);
}

//------

CJObjTypeP CSVGJAnimatedAngleType::type_;

CJObjTypeP
CSVGJAnimatedAngleType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CSVGJAnimatedAngleType(js));

  return type_;
}

CSVGJAnimatedAngleType::
CSVGJAnimatedAngleType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAnimatedAngle")
{
}

//------

/*
  interface SVGAnimatedAngle {
    readonly attribute boolean baseVal;
    readonly attribute boolean animVal;
  };
*/
CSVGJAnimatedAngle::
CSVGJAnimatedAngle(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CSVGJObjValue(svg, obj, name, CSVGJAnimatedAngleType::instance(svg->js()))
{
}

CJValueP
CSVGJAnimatedAngle::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

CJValueP
CSVGJAnimatedAngle::
getProperty(CJavaScript *js, const std::string &name) const
{
  // readonly attribute boolean animVal
  if      (name == "animVal") {
    COptString str = obj_->getNameValue(name_);

    COptValT<CAngle> angle = CSVG::decodeAngleString(str.getValue(""));

    return CJValueP(new CSVGJAngle(svg_, angle.getValue(CAngle())));
  }
  // attribute boolean baseVal
  else if (name == "baseVal") {
    return CJValueP(new CSVGJAngle(svg_, obj_, name_));
  }

  return CSVGJObjValue::getProperty(js, name);
}
