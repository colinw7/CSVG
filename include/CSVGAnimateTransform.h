#ifndef CSVGAnimateTransform_H
#define CSVGAnimateTransform_H

#include <CSVGAnimateBase.h>

class CSVGAnimateTransform : public CSVGAnimateBase {
 public:
  CSVG_OBJECT_DEF("animateTransform", CSVGObjTypeId::ANIMATE_TRANSFORM)

  CSVGAnimateTransform(CSVG &svg);
  CSVGAnimateTransform(const CSVGAnimateTransform &animateTransform);

  CSVGAnimateTransform *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGAnimateTransform &animateTransform);

 private:
  COptValT<std::string> type_;
  COptValT<std::string> additive_;
};

#endif
