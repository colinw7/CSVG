#ifndef CSVGFeGaussianBlur_H
#define CSVGFeGaussianBlur_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeGaussianBlur : public CSVGFilterBase, public CSVGPrintBase<CSVGFeGaussianBlur> {
 public:
  CSVG_OBJECT_DEF("feGaussianBlur", CSVGObjTypeId::FE_GAUSSIAN_BLUR)

  CSVGFeGaussianBlur(CSVG &svg);
  CSVGFeGaussianBlur(const CSVGFeGaussianBlur &blur);

  CSVGFeGaussianBlur *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  double getStdDevX() const { return stdDevX_.getValue(0); }
  void setStdDevX(double d) { stdDevX_ = d; }

  double getStdDevY() const { return stdDevY_.getValue(getStdDevX()); }
  void setStdDevY(double d) { stdDevX_ = d; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptString filterIn_;
  COptString filterOut_;
  COptReal   stdDevX_;
  COptReal   stdDevY_;
};

#endif
