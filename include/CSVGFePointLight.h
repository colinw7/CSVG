#ifndef CSVGFePointLight_H
#define CSVGFePointLight_H

#include <CSVGFilter.h>

class CSVGFePointLight : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("fePointLight", CSVGObjTypeId::FE_POINT_LIGHT)

  CSVGFePointLight(CSVG &svg);
  CSVGFePointLight(const CSVGFePointLight &fe);

  CSVGFePointLight *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFePointLight &fe);
};

#endif
