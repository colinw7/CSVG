#ifndef CSVGFeBlend_H
#define CSVGFeBlend_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeBlend : public CSVGFilterBase, public CSVGPrintBase<CSVGFeBlend> {
 public:
  CSVG_OBJECT_DEF("feBlend", CSVGObjTypeId::FE_BLEND)

  CSVGFeBlend(CSVG &svg);
  CSVGFeBlend(const CSVGFeBlend &fe);

  CSVGFeBlend *dup() const override;

  CSVGBlendMode getMode() const { return mode_.getValue(CSVGBlendMode::NORMAL); }
  void setMode(CSVGBlendMode m) { mode_ = m; }

  std::string getFilterIn1() const;
  void setFilterIn1(const std::string &s) { filterIn1_ = s; }

  std::string getFilterIn2() const;
  void setFilterIn2(const std::string &s) { filterIn2_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptValT<CSVGBlendMode> mode_;
  COptString              filterIn1_;
  COptString              filterIn2_;
  COptString              filterOut_;
};

#endif
