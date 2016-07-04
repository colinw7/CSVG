#ifndef CSVGColorProfile_H
#define CSVGColorProfile_H

#include <CSVGObject.h>

class CSVGColorProfile : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("color-profile", CSVGObjTypeId::COLOR_PROFILE)

  CSVGColorProfile(CSVG &svg);
  CSVGColorProfile(const CSVGColorProfile &c);

  CSVGColorProfile *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGColorProfile &face);

 private:
  COptString          local_;
  COptString          name_;
  COptString          intent_;
  COptValT<CSVGXLink> xlink_;
};

#endif
