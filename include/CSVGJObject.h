#ifndef CSVGJObject_H
#define CSVGJObject_H

#include <CJavaScript.h>

class CSVGObject;

class CSVGJObjectType : public CJObjectType {
 public:
  CSVGJObjectType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJObject : public CJObject {
 public:
  CSVGJObject(CSVGObject *obj);

  CSVGObject *obj() const { return obj_; }

  CJValue *dup(CJavaScript *) const override { return new CSVGJObject(obj_); }

  std::string toString() const override;

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP getProperty(const std::string &key) const;
  void setProperty(const std::string &key, CJValueP value);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override;

 private:
  CSVGObject *obj_;
};

typedef std::shared_ptr<CSVGJObject> CSVGJObjectP;

#endif
