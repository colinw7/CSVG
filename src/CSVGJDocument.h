#ifndef CSVGJDocument_H
#define CSVGJDocument_H

#include <CSVG.h>
#include <CJavaScript.h>

class CSVGJDocumentType : public CJObjectType {
 public:
  CSVGJDocumentType();

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }
};

class CSVGJDocument : public CJObject {
 public:
  CSVGJDocument(CSVG *svg);

  CJValue *dup(CJavaScript *) const override { return new CSVGJDocument(svg_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "SVG"; }

 private:
  CSVG *svg_;
};

#endif
