#ifndef CSVGJTransform_H
#define CSVGJTransform_H

#include <CMatrixStack2D.h>
#include <CJavaScript.h>

class CSVG;
class CSVGObject;
class CSVGTransform;

class CSVGJTransformStackType : public CJObjectType {
 public:
  CSVGJTransformStackType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJTransformStack : public CJObject {
 public:
  CSVGJTransformStack(CSVG *svg, CSVGObject *obj, bool baseVal=false);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJTransformStack(svg_, obj_, baseVal_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  bool hasIndex() const override { return true; }

  int length() const override;

  CJValueP indexValue(int i) const override;

  CMatrixStack2D matrixStack() const;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "SVGTransformStack: " << matrixStack(); }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  bool        baseVal_ { false };
};

//------

class CSVGJTransformType : public CJObjectType {
 public:
  CSVGJTransformType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJTransform : public CJObject {
 public:
  CSVGJTransform(CSVG *svg, CSVGObject *obj, bool baseVal, int ind);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJTransform(svg_, obj_, baseVal_, ind_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CMatrixStack2D::Transform transform() const;
  void setTransform(const CMatrixStack2D::Transform &transform);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "SVGTransform: " << transform(); }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  bool        baseVal_ { false };
  int         ind_ { 0 };
};

//------

class CSVGJMatrixType : public CJObjectType {
 public:
  CSVGJMatrixType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJMatrix : public CJObject {
 public:
  CSVGJMatrix(CSVG *svg, CSVGObject *obj, bool baseVal, int ind);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJMatrix(svg_, obj_, baseVal_, ind_);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CMatrix2D matrix() const;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "SVGMatrix: " << matrix(); }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  bool        baseVal_ { false };
  int         ind_ { 0 };
};

#endif
