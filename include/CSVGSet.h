#ifndef CSVGSet_H
#define CSVGSet_H

#include <CSVGAnimateBase.h>

class CSVGSet : public CSVGAnimateBase {
 public:
  CSVG_OBJECT_DEF("set", CSVGObjTypeId::SET)

  CSVGSet(CSVG &svg);
  CSVGSet(const CSVGSet &set);

  CSVGSet *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGSet &set);
};

#endif
