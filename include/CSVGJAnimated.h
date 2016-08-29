#ifndef CSVGJAnimated_H
#define CSVGJAnimated_H

#include <CSVG.h>
#include <CJavaScript.h>

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
class CSVGJAnimatedInteger : public CJObj {
 public:
  CSVGJAnimatedInteger(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedInteger(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedInteger"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
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
class CSVGJAnimatedNumber : public CJObj {
 public:
  CSVGJAnimatedNumber(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedNumber(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedNumber"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
};

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
class CSVGJAnimatedNumberList : public CJObj {
 public:
  CSVGJAnimatedNumberList(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedNumberList(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedNumberList"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
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
class CSVGJAnimatedLength : public CJObj {
 public:
  CSVGJAnimatedLength(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedLength(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedLength"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
};

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
class CSVGJAnimatedLengthList : public CJObj {
 public:
  CSVGJAnimatedLengthList(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedLengthList(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedLengthList"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
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
class CSVGJAnimatedRect : public CJObj {
 public:
  CSVGJAnimatedRect(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedRect(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedRect"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
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
class CSVGJAnimatedTransformList : public CJObj {
 public:
  CSVGJAnimatedTransformList(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedTransformList(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedTransformList"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
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
class CSVGJAnimatedAngle : public CJObj {
 public:
  CSVGJAnimatedAngle(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedAngle(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedAngle"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
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
class CSVGJAnimatedPreserveAspectRatio : public CJObj {
 public:
  CSVGJAnimatedPreserveAspectRatio(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJAnimatedPreserveAspectRatio(svg_, obj_, name_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP getProperty(CJavaScript *, const std::string &) const override;

  void print(std::ostream &os) const override { os << "SVGAnimatedPreserveAspectRatio"; }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  std::string name_;
};

#endif
