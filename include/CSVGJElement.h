#ifndef CSVGJElement_H
#define CSVGJElement_H

#ifdef CSVG_JAVASCRIPT

#include <CJavaScript.h>

class CSVGObject;

class CSVGJElementType : public CJObjType {
 public:
  CSVGJElementType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJElement : public CJObj {
 public:
  CSVGJElement(CSVGObject *obj);

  CSVGObject *obj() const { return obj_; }

  CJValue *dup(CJavaScript *) const override { return new CSVGJElement(obj_); }

  std::string toString() const override;

  std::optional<double> toReal() const override { return std::optional<double>(); }

  bool toBoolean() const override { return false; }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override;

 private:
  CSVGObject *obj_;
  CJValueP    data_;
};

using CSVGJElementP = std::shared_ptr<CSVGJElement>;

#endif

#endif
