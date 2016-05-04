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

  std::string getOrder() const { return order_.getValue("3"); }
  void setOrder(const std::string &s) { order_ = s; }

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

  std::string edgeMode() const { return edgeMode_.getValue("duplicate"); }
  void setEdgeMode(const std::string &s) { edgeMode_ = s; }

  std::string kernelUnitLength() const { return kernelUnitLength_.getValue("1"); }
  void setKernelUnitLength(const std::string &s) { kernelUnitLength_ = s; }

  std::string preserveAlpha() const { return preserveAlpha_.getValue("false"); }
  void setPreserveAlpha(const std::string &s) { preserveAlpha_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void filterImage(CSVGBuffer *inBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeConvolveMatrix &filter);

 private:
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
