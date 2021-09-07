#ifndef CSVGColorProfile_H
#define CSVGColorProfile_H

#include <CSVGObject.h>

class CSVGColorProfile : public CSVGObject, public CSVGPrintBase<CSVGColorProfile> {
 public:
  CSVG_OBJECT_DEF("color-profile", CSVGObjTypeId::COLOR_PROFILE)

  CSVGColorProfile(CSVG &svg);
  CSVGColorProfile(const CSVGColorProfile &c);

  CSVGColorProfile *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptString          local_;
  COptString          name_;
  COptString          intent_;
  COptValT<CSVGXLink> xlink_;
};

#endif
