#ifndef CSVGFeFlood_H
#define CSVGFeFlood_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeFlood : public CSVGFilterBase, public CSVGPrintBase<CSVGFeFlood> {
 public:
  CSVG_OBJECT_DEF("feFlood", CSVGObjTypeId::FE_FLOOD)

  CSVGFeFlood(CSVG &svg);
  CSVGFeFlood(const CSVGFeFlood &fe);

  CSVGFeFlood *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  double getOpacity() const { return opacity_.getValue(1); }
  void setOpacity(double r) { opacity_ = r; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptString      filterIn_;
  COptString      filterOut_;
  COptValT<Color> color_;
  COptReal        opacity_;
};

#endif
