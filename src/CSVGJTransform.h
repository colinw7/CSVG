#ifndef CSVGJTransform_H
#define CSVGJTransform_H

#include <CJavaScript.h>
#include <CMatrixStack2D.h>

class CSVG;
class CSVGTransform;

class CSVGJTransformStackType : public CJObjectType {
 public:
  CSVGJTransformStackType() :
   CJObjectType(CJToken::Type::Object, "SVGTransformStack") {
  }

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJTransformStack : public CJObject {
 public:
  CSVGJTransformStack(CSVG *svg, const CMatrixStack2D &matrix);

  CJValue *dup(CJavaScript *) const override {
    return new CSVGJTransformStack(svg_, transformStack_);
  }

  std::string toString() const override { return "SVGTransformStack"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  bool hasIndex() const override { return true; }

  int length() const override;

  CJValueP indexValue(int i) const override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const { os << "SVGTransformStack"; }

 private:
  CSVG*          svg_;
  CMatrixStack2D transformStack_;
};

//------

class CSVGJTransformType : public CJObjectType {
 public:
  CSVGJTransformType() :
   CJObjectType(CJToken::Type::Object, "SVGTransform") {
  }

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJTransform : public CJObject {
 public:
  CSVGJTransform(CSVG *svg, const CMatrixStack2D::Transform &transform);

  CJValue *dup(CJavaScript *) const override { return new CSVGJTransform(svg_, transform_); }

  std::string toString() const override { return "SVGTransform"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const { os << "SVGTransform"; }

 private:
  CSVG*                     svg_;
  CMatrixStack2D::Transform transform_;
};

//------

class CSVGJMatrixType : public CJObjectType {
 public:
  CSVGJMatrixType() :
   CJObjectType(CJToken::Type::Object, "SVGMatrix") {
  }

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJMatrix : public CJObject {
 public:
  CSVGJMatrix(CSVG *svg, const CMatrix2D &matrix);

  CJValue *dup(CJavaScript *) const override { return new CSVGJMatrix(svg_, matrix_); }

  std::string toString() const override { return "SVGMatrix"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const { os << "SVGMatrix"; }

 private:
  CSVG*     svg_;
  CMatrix2D matrix_;
};

#endif
