#ifndef CSVGAnimateTransform_H
#define CSVGAnimateTransform_H

#include <CSVGAnimateBase.h>

class CSVGAnimateTransform : public CSVGAnimateBase, public CSVGPrintBase<CSVGAnimateTransform> {
 public:
  CSVG_OBJECT_DEF("animateTransform", CSVGObjTypeId::ANIMATE_TRANSFORM)

  CSVGAnimateTransform(CSVG &svg);
  CSVGAnimateTransform(const CSVGAnimateTransform &animateTransform);

  CSVGAnimateTransform *dup() const override;

  std::string getType() const { return type_.getValue(""); }
  void setType(const std::string &s) { type_ = s; }

  std::string getAdditive() const { return additive_.getValue(""); }
  void setAdditive(const std::string &s) { additive_ = s; }

  std::string getAccumulate() const { return accumulate_.getValue(""); }
  void setAccumulate(const std::string &s) { accumulate_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t) override;

  void print(std::ostream &os, bool hier=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptString type_;
  COptString additive_;
  COptString accumulate_;
  COptString calcMode_;
  COptString keySplines_;
};

#endif
