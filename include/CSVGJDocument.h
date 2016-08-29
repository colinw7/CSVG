#ifndef CSVGJDocument_H
#define CSVGJDocument_H

#include <CSVG.h>
#include <CJavaScript.h>

class CSVGJDocumentType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJDocumentType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJDocument : public CJObj {
 public:
  CSVGJDocument(CSVG *svg);

  CJValue *dup(CJavaScript *) const override { return new CSVGJDocument(svg_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "SVG"; }

 private:
  CSVG*    svg_;
  CJValueP element_;
};

//------

class CSVGJDocumentElementType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CSVGJDocumentElementType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

class CSVGJDocumentElement : public CJObj {
 public:
  CSVGJDocumentElement(CSVG *svg);

  CJValue *dup(CJavaScript *) const override { return new CSVGJDocumentElement(svg_); }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "SVGDocumentElement"; }

 private:
  CSVG *svg_;
};

#endif
