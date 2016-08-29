#ifndef CSVGJEvent_H
#define CSVGJEvent_H

#include <CSVG.h>
#include <CJavaScript.h>

class CSVGJEventType : public CJObjType {
 public:
  CSVGJEventType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJEvent : public CJObj {
 public:
  CSVGJEvent(CSVG *svg);

  CJValue *dup(CJavaScript *) const override { return new CSVGJEvent(svg_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "SVGEvent"; }

 private:
  CSVG *svg_;
};

#endif
