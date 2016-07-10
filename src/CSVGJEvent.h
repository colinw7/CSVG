#ifndef CSVGJEvent_H
#define CSVGJEvent_H

#include <CJavaScript.h>
#include <CSVG.h>

class CSVG;

class CSVGJEventType : public CJObjectType {
 public:
  CSVGJEventType() :
   CJObjectType(CJToken::Type::Object, "SVGEvent") {
  }

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJEvent : public CJObject {
 public:
  CSVGJEvent(CSVG *svg);

  CJValue *dup(CJavaScript *) const override { return new CSVGJEvent(svg_); }

  std::string toString() const override { return "SVGEvent"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const { os << "SVGEvent"; }

 private:
  CSVG *svg_;
};

#endif
