#ifndef CSVGJObject_H
#define CSVGJObject_H

#include <CJavaScript.h>

class CSVGObject;

class CSVGJObjectType : public CJObjectType {
 public:
  CSVGJObjectType() :
   CJObjectType(CJToken::Type::Object, "SVGObject") {
  }

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJObject : public CJObject {
 public:
  CSVGJObject(CSVGObject *obj);

  CJValue *dup(CJavaScript *) const override { return new CSVGJObject(obj_); }

  std::string toString() const override { return "SVGObject"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const { os << "SVGObject"; }

 private:
  CSVGObject *obj_;
};

#endif
