#ifndef CSVGFeConvolveMatrix_H
#define CSVGFeConvolveMatrix_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeConvolveMatrix : public CSVGFilterBase {
 public:
  using Reals = std::vector<double>;

 public:
  CSVG_OBJECT_DEF("feConvolveMatrix", CSVGObjTypeId::FE_CONVOLVE_MATRIX)

  CSVGFeConvolveMatrix(CSVG &svg);
  CSVGFeConvolveMatrix(const CSVGFeConvolveMatrix &merge);

  CSVGFeConvolveMatrix *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  Reals getOrder() const { return order_.getValue(Reals()); }
  void setOrder(const Reals &r) { order_ = r; }

  Reals getKernelMatrix() const { return kernelMatrix_.getValue(Reals()); }
  void setKernelMatrix(const Reals &r) { kernelMatrix_ = r; }

  double getDivisor() const { return divisor_.getValue(1); }
  void setDivisor(double r) { divisor_ = r; }

  double getBias() const { return bias_.getValue(0); }
  void setBias(double r) { bias_ = r; }

  double getTargetX() const { return targetX_.getValue(0); }
  void setTargetX(double r) { targetX_ = r; }

  double getTargetY() const { return targetY_.getValue(0); }
  void setTargetY(double r) { targetY_ = r; }

  std::string getEdgeMode() const { return edgeMode_.getValue("duplicate"); }
  void setEdgeMode(const std::string &s) { edgeMode_ = s; }

  std::string getKernelUnitLength() const { return kernelUnitLength_.getValue("1"); }
  void setKernelUnitLength(const std::string &s) { kernelUnitLength_ = s; }

  std::string getPreserveAlpha() const { return preserveAlpha_.getValue("false"); }
  void setPreserveAlpha(const std::string &s) { preserveAlpha_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeConvolveMatrix &filter);

 private:
  COptString      filterIn_;
  COptValT<Reals> order_;
  COptValT<Reals> kernelMatrix_;
  COptReal        divisor_;
  COptReal        bias_;
  COptReal        targetX_;
  COptReal        targetY_;
  COptString      edgeMode_;
  COptString      kernelUnitLength_;
  COptString      preserveAlpha_;
};

#endif
