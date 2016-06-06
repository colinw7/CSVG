#ifndef CSVGAnimateColor_H
#define CSVGAnimateColor_H

#include <CSVGAnimateBase.h>

class CSVGAnimateColor : public CSVGAnimateBase {
 public:
  CSVG_OBJECT_DEF("animateColor", CSVGObjTypeId::ANIMATE_COLOR)

  CSVGAnimateColor(CSVG &svg);
  CSVGAnimateColor(const CSVGAnimateColor &animateColor);

  CSVGAnimateColor *dup() const override;

  std::string getType() const { return type_.getValue(""); }

  std::string getAdditive() const { return additive_.getValue(""); }

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGAnimateColor &animateColor);

 private:
  COptString type_;
  COptString additive_;
};

#endif
