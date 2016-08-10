#ifndef CSVGJEvent_H
#define CSVGJEvent_H

#include <CSVG.h>
#include <CJavaScript.h>

class CSVGJEventType : public CJObjectType {
 public:
  CSVGJEventType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJEvent : public CJObject {
 public:
  CSVGJEvent(CSVG *svg);

  CJValue *dup(CJavaScript *) const override { return new CSVGJEvent(svg_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "SVGEvent"; }

 private:
  CSVG *svg_;
};

#endif
