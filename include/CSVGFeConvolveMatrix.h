#ifndef CSVGFeConvolveMatrix_H
#define CSVGFeConvolveMatrix_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeConvolveMatrix : public CSVGFilterBase {
 public:
  typedef std::vector<double> Reals;

 public:
  CSVG_OBJECT_DEF("feConvolveMatrix", CSVGObjTypeId::FE_CONVOLVE_MATRIX)

  CSVGFeConvolveMatrix(CSVG &svg);
  CSVGFeConvolveMatrix(const CSVGFeConvolveMatrix &merge);

  CSVGFeConvolveMatrix *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("SourceGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void filterImage(CSVGBuffer *inBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeConvolveMatrix &filter);

 private:
  COptValT<std::string> class_;
  COptValT<std::string> style_;
  COptValT<std::string> filterIn_;
  COptValT<std::string> order_;
  COptValT<Reals>       kernelMatrix_;
  COptValT<double>      divisor_;
  COptValT<double>      bias_;
  COptValT<double>      targetX_;
  COptValT<double>      targetY_;
  COptValT<std::string> edgeMode_;
  COptValT<std::string> kernelUnitLength_;
  COptValT<std::string> preserveAlpha_;
};

#endif
