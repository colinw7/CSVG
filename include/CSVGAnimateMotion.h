#ifndef CSVGAnimateMotion_H
#define CSVGAnimateMotion_H

#include <CSVGAnimateBase.h>

class CSVGAnimateMotion : public CSVGAnimateBase {
 public:
  typedef std::vector<CSVGPathPart *> PartList;

 public:
  CSVG_OBJECT_DEF("animateMotion", CSVGObjTypeId::ANIMATE_MOTION)

  CSVGAnimateMotion(CSVG &svg);
  CSVGAnimateMotion(const CSVGAnimateMotion &animateMotion);

  CSVGAnimateMotion *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGAnimateMotion &animateMotion);

 private:
  PartList              path_;
  COptValT<std::string> keyPoints_;
  COptValT<std::string> rotate_;
  COptValT<std::string> origin_;
};

#endif
