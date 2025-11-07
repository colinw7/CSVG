#ifndef CSVGFeConvolveMatrix_H
#define CSVGFeConvolveMatrix_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeConvolveMatrix : public CSVGFilterBase, public CSVGPrintBase<CSVGFeConvolveMatrix> {
 public:
  using Reals = std::vector<double>;

 public:
  CSVG_OBJECT_DEF("feConvolveMatrix", CSVGObjTypeId::FE_CONVOLVE_MATRIX)

  CSVGFeConvolveMatrix(CSVG &svg);
  CSVGFeConvolveMatrix(const CSVGFeConvolveMatrix &merge);

  CSVGFeConvolveMatrix *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  Reals getOrder() const { return order_.value_or(Reals()); }
  void setOrder(const Reals &r) { order_ = r; }

  Reals getKernelMatrix() const { return kernelMatrix_.value_or(Reals()); }
  void setKernelMatrix(const Reals &r) { kernelMatrix_ = r; }

  double getDivisor() const { return divisor_.value_or(1); }
  void setDivisor(double r) { divisor_ = r; }

  double getBias() const { return bias_.value_or(0); }
  void setBias(double r) { bias_ = r; }

  double getTargetX() const { return targetX_.value_or(0); }
  void setTargetX(double r) { targetX_ = r; }

  double getTargetY() const { return targetY_.value_or(0); }
  void setTargetY(double r) { targetY_ = r; }

  std::string getEdgeMode() const { return edgeMode_.value_or("duplicate"); }
  void setEdgeMode(const std::string &s) { edgeMode_ = s; }

  std::string getKernelUnitLength() const { return kernelUnitLength_.value_or("1"); }
  void setKernelUnitLength(const std::string &s) { kernelUnitLength_ = s; }

  std::string getPreserveAlpha() const { return preserveAlpha_.value_or("false"); }
  void setPreserveAlpha(const std::string &s) { preserveAlpha_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<std::string> filterIn_;
  std::optional<Reals>       order_;
  std::optional<Reals>       kernelMatrix_;
  std::optional<double>      divisor_;
  std::optional<double>      bias_;
  std::optional<double>      targetX_;
  std::optional<double>      targetY_;
  std::optional<std::string> edgeMode_;
  std::optional<std::string> kernelUnitLength_;
  std::optional<std::string> preserveAlpha_;
};

#endif
