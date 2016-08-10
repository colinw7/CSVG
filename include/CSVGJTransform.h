#ifndef CSVGJTransform_H
#define CSVGJTransform_H

#include <CMatrixStack2D.h>
#include <CJavaScript.h>

class CSVG;
class CSVGObject;
class CSVGTransform;
class CSVGJMatrix;

typedef std::shared_ptr<CSVGJMatrix> CSVGJMatrixP;

//------

class CSVGJTransformStackType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CSVGJTransformStackType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjectTypeP type_;
};

class CSVGJTransformStack : public CJObject {
 public:
  CSVGJTransformStack(CSVG *svg, CSVGObject *obj, bool baseVal=false);
  CSVGJTransformStack(CSVG *svg, const CMatrixStack2D &stack);

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

  CJValueP getProperty(const std::string &key) const;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "SVGTransformStack: " << matrixStack(); }

 private:
  CSVG*          svg_ { 0 };
  CSVGObject*    obj_ { 0 };
  bool           baseVal_ { false };
  CMatrixStack2D stack_;
};

//------

class CSVGJTransformType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CSVGJTransformType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjectTypeP type_;
};

class CSVGJTransform : public CJObject {
 public:
  CSVGJTransform(CSVG *svg, CSVGObject *obj, bool baseVal, int ind);

  CSVGJTransform(CSVG *svg, CSVGJMatrixP matrix);

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

  CJValueP getProperty(const std::string &key) const;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "SVGTransform: " << transform(); }

 private:
  CSVG*        svg_ { 0 };
  CSVGObject*  obj_ { 0 };
  bool         baseVal_ { false };
  int          ind_ { 0 };
  CSVGJMatrixP matrix_;
};

//------

class CSVGJMatrixType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CSVGJMatrixType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjectTypeP type_;
};

class CSVGJMatrix : public CJObject {
 public:
  CSVGJMatrix(CSVG *svg, CSVGObject *obj, bool baseVal, int ind);
  CSVGJMatrix(CSVG *svg, const CMatrix2D &m);

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
  void setMatrix(const CMatrix2D &m);

  int cmp(CJObjectP obj) const;

  CJValueP getProperty(const std::string &key) const override;
  void     setProperty(const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "SVGMatrix: " << matrix(); }

 private:
  CSVG*       svg_ { 0 };
  CSVGObject* obj_ { 0 };
  bool        baseVal_ { false };
  int         ind_ { 0 };
  CMatrix2D   m_;
};

#endif
