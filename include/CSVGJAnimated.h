#ifndef CSVGJAnimated_H
#define CSVGJAnimated_H

#ifdef CSVG_JAVASCRIPT

#include <CSVG.h>
#include <CJavaScript.h>

//------

class CSVGJObjValue : public CJObj {
 public:
  CSVGJObjValue(CSVG *svg, CSVGObject *obj, const std::string &name, CJObjTypeP type);

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

 protected:
  CSVG*       svg_ { nullptr };
  CSVGObject* obj_ { nullptr };
  std::string name_;
};

//------

// SVGAnimatedInteger Type
class CSVGJAnimatedIntegerType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedIntegerType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedInteger Value
class CSVGJAnimatedInteger : public CSVGJObjValue {
 public:
  CSVGJAnimatedInteger(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedInteger(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedInteger"; }
};

//------

// SVGAnimatedNumber Type
class CSVGJAnimatedNumberType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedNumberType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedNumber Value
class CSVGJAnimatedNumber : public CSVGJObjValue {
 public:
  CSVGJAnimatedNumber(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedNumber(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedNumber"; }
};

//------

// SVGAnimatedNumberList Type
class CSVGJAnimatedNumberListType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedNumberListType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedNumberList Value
class CSVGJAnimatedNumberList : public CSVGJObjValue {
 public:
  CSVGJAnimatedNumberList(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedNumberList(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedNumberList"; }
};

//------

// SVGAnimatedLength Type
class CSVGJAnimatedLengthType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedLengthType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedLength Value
class CSVGJAnimatedLength : public CSVGJObjValue {
 public:
  CSVGJAnimatedLength(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedLength(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedLength"; }
};

//------

// SVGAnimatedLengthList Type
class CSVGJAnimatedLengthListType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedLengthListType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedLengthList Value
class CSVGJAnimatedLengthList : public CSVGJObjValue {
 public:
  CSVGJAnimatedLengthList(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedLengthList(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedLengthList"; }
};

//------

// SVGAnimatedString Type
class CSVGJAnimatedStringType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedStringType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedString Value
class CSVGJAnimatedString : public CSVGJObjValue {
 public:
  CSVGJAnimatedString(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedString(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedString"; }
};

//------

// SVGAnimatedRect Type
class CSVGJAnimatedRectType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedRectType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedRect Value
class CSVGJAnimatedRect : public CSVGJObjValue {
 public:
  CSVGJAnimatedRect(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedRect(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedRect"; }
};

//------

// SVGAnimatedTransformList Type
class CSVGJAnimatedTransformListType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedTransformListType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedTransformList Value
class CSVGJAnimatedTransformList : public CSVGJObjValue {
 public:
  CSVGJAnimatedTransformList(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedTransformList(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedTransformList"; }
};

//------

// SVGAnimatedAngle Type
class CSVGJAnimatedAngleType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedAngleType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedAngle Value
class CSVGJAnimatedAngle : public CSVGJObjValue {
 public:
  CSVGJAnimatedAngle(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedAngle(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedAngle"; }
};

//------

// SVGAnimatedPreserveAspectRatio Type
class CSVGJAnimatedPreserveAspectRatioType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedPreserveAspectRatioType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedPreserveAspectRatio Value
class CSVGJAnimatedPreserveAspectRatio : public CSVGJObjValue {
 public:
  CSVGJAnimatedPreserveAspectRatio(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedPreserveAspectRatio(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedPreserveAspectRatio"; }
};

//------

// SVGPreserveAspectRatio Type
class CSVGJPreserveAspectRatioType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJPreserveAspectRatioType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGPreserveAspectRatio Value
class CSVGJPreserveAspectRatio : public CJObj {
 public:
  CSVGJPreserveAspectRatio(CSVG *svg, const CSVGPreserveAspect &preserveAspect);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJPreserveAspectRatio(svg_, preserveAspect_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGPreserveAspectRatio"; }

 protected:
  CSVG*              svg_ { nullptr };
  CSVGPreserveAspect preserveAspect_;
};

//------

// SVGAnimatedEnumeration Type
class CSVGJAnimatedEnumerationType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedEnumerationType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedEnumeration Value
class CSVGJAnimatedEnumeration : public CSVGJObjValue {
 public:
  CSVGJAnimatedEnumeration(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedEnumeration(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedEnumeration"; }
};

//------

// SVGAnimatedBoolean Type
class CSVGJAnimatedBooleanType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAnimatedBooleanType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

// SVGAnimatedBoolean Value
class CSVGJAnimatedBoolean : public CSVGJObjValue {
 public:
  CSVGJAnimatedBoolean(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedBoolean(svg_, obj_, name_); }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedBoolean"; }
};

#endif

#endif
