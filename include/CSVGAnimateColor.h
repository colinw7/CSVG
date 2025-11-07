#ifndef CSVGAnimateColor_H
#define CSVGAnimateColor_H

#include <CSVGAnimateBase.h>

class CSVGAnimateColor : public CSVGAnimateBase, public CSVGPrintBase<CSVGAnimateColor> {
 public:
  CSVG_OBJECT_DEF("animateColor", CSVGObjTypeId::ANIMATE_COLOR)

  CSVGAnimateColor(CSVG &svg);
  CSVGAnimateColor(const CSVGAnimateColor &animateColor);

  CSVGAnimateColor *dup() const override;

  std::string getType() const { return type_.value_or(""); }

  std::string getAdditive() const { return additive_.value_or(""); }

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t) override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<std::string> type_;
  std::optional<std::string> additive_;
};

#endif
