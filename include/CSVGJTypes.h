#ifndef CSVGJTypes_H
#define CSVGJTypes_H

#include <CSVGTypes.h>
#include <CSVGColor.h>
#include <CJavaScript.h>
#include <CMatrixStack2D.h>
#include <CBBox2D.h>
#include <CScreenUnits.h>
#include <CAngle.h>

class CSVG;
class CSVGObject;
class CSVGTransform;

class CSVGJTransformList;
class CSVGJMatrix;
class CSVGJRect;
class CSVGJNumberList;
class CSVGJNumber;
class CSVGJLengthList;
class CSVGJLength;
class CSVGJStringList;
class CSVGJAngle;
class CSVGJColor;

typedef std::shared_ptr<CSVGJTransformList> CSVGJTransformListP;
typedef std::shared_ptr<CSVGJMatrix>        CSVGJMatrixP;
typedef std::shared_ptr<CSVGJRect>          CSVGJRectP;
typedef std::shared_ptr<CSVGJNumberList>    CSVGJNumberListP;
typedef std::shared_ptr<CSVGJNumber>        CSVGJNumberP;
typedef std::shared_ptr<CSVGJLengthList>    CSVGJLengthListP;
typedef std::shared_ptr<CSVGJLength>        CSVGJLengthP;
typedef std::shared_ptr<CSVGJStringList>    CSVGJStringListP;
typedef std::shared_ptr<CSVGJAngle>         CSVGJAngleP;
typedef std::shared_ptr<CSVGJColor>         CSVGJColorP;

//------

class CSVGJTransformListType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJTransformListType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJTransformList : public CJObj {
 public:
  CSVGJTransformList(CSVG *svg, CSVGObject *obj);
  CSVGJTransformList(CSVG *svg, const CMatrixStack2D &stack);

  CJValue *dup(CJavaScript *) const override {
    if (obj_)
      return new CSVGJTransformList(svg_, obj_);
    else
      return new CSVGJTransformList(svg_, stack_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  bool hasIndex() const override { return true; }

  COptLong length() const override;

  CJValueP indexValue(long i) const override;

  CMatrixStack2D matrixStack() const;

  CMatrix2D matrix() const;
  void setMatrix(const CMatrix2D &m);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "SVGTransformList: " << matrixStack(); }

 private:
  CSVG*          svg_ { 0 };
  CSVGObject*    obj_ { 0 };
  CMatrixStack2D stack_;
};

//------

class CSVGJTransformType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJTransformType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJTransform : public CJObj {
 public:
  CSVGJTransform(CSVG *svg, CSVGObject *obj, int ind);

  CSVGJTransform(CSVG *svg, CSVGJMatrixP matrix);

  CJValue *dup(CJavaScript *) const override {
    if (obj_)
      return new CSVGJTransform(svg_, obj_, ind_);
    else
      return new CSVGJTransform(svg_, matrix_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  CMatrixStack2D::Transform transform() const;
  void setTransform(const CMatrixStack2D::Transform &transform);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "SVGTransform: " << transform(); }

 private:
  CSVG*        svg_ { 0 };
  CSVGObject*  obj_ { 0 };
  int          ind_ { 0 };
  CSVGJMatrixP matrix_;
};

//------

class CSVGJMatrixType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJMatrixType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJMatrix : public CJObj {
 public:
  CSVGJMatrix(CSVG *svg, CSVGObject *obj, int ind);
  CSVGJMatrix(CSVG *svg, const CSVGJTransformListP &stack);
  CSVGJMatrix(CSVG *svg, const CMatrix2D &m);

  CJValue *dup(CJavaScript *) const override {
    if (obj_)
      return new CSVGJMatrix(svg_, obj_, ind_);
    else
      return new CSVGJMatrix(svg_, stack_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  CMatrix2D matrix() const;
  void setMatrix(const CMatrix2D &m);

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG*               svg_ { 0 };
  CSVGObject*         obj_ { 0 };
  int                 ind_ { 0 };
  CSVGJTransformListP stack_;
  CMatrix2D           m_;
};

//------

class CSVGJRectType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJRectType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJRect : public CJObj {
 public:
  CSVGJRect(CSVG *svg, const CBBox2D &rect);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJRect(svg_, rect_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  CBBox2D rect() const;
  void setRect(const CBBox2D &rect);

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG*   svg_ { 0 };
  CBBox2D rect_;
};

//------

class CSVGJNumberListType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJNumberListType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJNumberList : public CJObj {
 public:
  typedef std::vector<double> Reals;

 public:
  CSVGJNumberList(CSVG *svg, const Reals &reals);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJNumberList(svg_, reals_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  const Reals &numberList() const;
  void setNumberList(const Reals &reals);

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG* svg_ { 0 };
  Reals reals_;
};

//------

class CSVGJNumberType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJNumberType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJNumber : public CJObj {
 public:
  CSVGJNumber(CSVG *svg, double r);

  CJValue *dup(CJavaScript *) const override { return new CSVGJNumber(svg_, r_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(r_); }

  bool toBoolean() const override { return (r_ != 0.0); }

  double number() const;
  void setNumber(double r);

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG*  svg_ { 0 };
  double r_ { 0.0 };
};

//------

class CSVGJLengthListType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJLengthListType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJLengthList : public CJObj {
 public:
  typedef std::vector<CScreenUnits> Lengths;

 public:
  CSVGJLengthList(CSVG *svg, const Lengths &lengths);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJLengthList(svg_, lengths_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  const Lengths &getLengths() const;
  void setLengths(const Lengths &lengths);

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG*   svg_ { 0 };
  Lengths lengths_;
};

//------

class CSVGJLengthType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJLengthType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJLength : public CJObj {
 public:
  CSVGJLength(CSVG *svg, CSVGObject *obj, const std::string &name);
  CSVGJLength(CSVG *svg, const CScreenUnits &length);

  CJValue *dup(CJavaScript *) const override {
    if (obj_)
      return new CSVGJLength(svg_, obj_, name_);
    else
      return new CSVGJLength(svg_, length_);
  }

  CScreenUnits getLength() const;

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG*        svg_ { 0 };
  CSVGObject*  obj_ { 0 };
  CScreenUnits length_;
  std::string  name_;
};

//------

class CSVGJStringListType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJStringListType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJStringList : public CJObj {
 public:
  typedef std::vector<std::string> Strings;

 public:
  CSVGJStringList(CSVG *svg, const Strings &reals);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJStringList(svg_, strings_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  const Strings &getStrings() const;
  void setStrings(const Strings &strings);

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG*   svg_ { 0 };
  Strings strings_;
};

//------

class CSVGJAngleType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJAngleType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJAngle : public CJObj {
 public:
  CSVGJAngle(CSVG *svg, const CAngle &angle);

  CSVGJAngle(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    if (obj_)
      return new CSVGJAngle(svg_, obj_, name_);
    else
      return new CSVGJAngle(svg_, angle_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(angle_.degrees()); }

  bool toBoolean() const override { return false; }

  CAngle getAngle() const;

  int typeValue() const {
    return typeValue(angle_);
  }

  static int typeValue(const CAngle &angle) {
    if      (angle.type() == CAngle::Type::DEGREES)
      return int(CSVGAngleType::DEG);
    else if (angle.type() == CAngle::Type::RADIANS)
      return int(CSVGAngleType::RAD);
    else if (angle.type() == CAngle::Type::GRADS)
      return int(CSVGAngleType::GRAD);
    else
      return int(CSVGAngleType::UNKNOWN);
  }

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  CAngle      angle_;
  std::string name_;
};

//------

class CSVGJColorType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJColorType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJColor : public CJObj {
 public:
  CSVGJColor(CSVG *svg, const CSVGColor &color);

  CSVGJColor(CSVG *svg, CSVGObject *obj, const std::string &name);

  CJValue *dup(CJavaScript *) const override {
    if (obj_)
      return new CSVGJColor(svg_, obj_, name_);
    else
      return new CSVGJColor(svg_, color_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  COptReal toReal() const override { return COptReal(); }

  bool toBoolean() const override { return false; }

  CSVGColor getColor() const;

  int cmp(CJObjP obj) const override;

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void     setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  CSVGColor   color_;
  std::string name_;
};

#endif
