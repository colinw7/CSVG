#ifndef CSVGFeDistantLight_H
#define CSVGFeDistantLight_H

#include <CSVGFilter.h>

class CSVGFeDistantLight : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feDistantLight", CSVGObjTypeId::FE_DISTANT_LIGHT)

  CSVGFeDistantLight(CSVG &svg);
  CSVGFeDistantLight(const CSVGFeDistantLight &fe);

  CSVGFeDistantLight *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeDistantLight &fe);
};

#endif
