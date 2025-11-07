#ifndef CSVGFeColorMatrix_H
#define CSVGFeColorMatrix_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeColorMatrix : public CSVGFilterBase, public CSVGPrintBase<CSVGFeColorMatrix> {
 public:
  using ValueList = std::vector<double>;

 public:
  CSVG_OBJECT_DEF("feColorMatrix", CSVGObjTypeId::FE_COLOR_MATRIX)

  CSVGFeColorMatrix(CSVG &svg);
  CSVGFeColorMatrix(const CSVGFeColorMatrix &blur);

  CSVGFeColorMatrix *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  CSVGColorMatrixType getType() const { return type_.value_or(CSVGColorMatrixType::MATRIX); }
  void setType(CSVGColorMatrixType t) { type_ = t; }

  const ValueList &getValues() const { return values_; }
  void setValues(const ValueList &values) { values_ = values; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<std::string>         filterIn_;
  std::optional<std::string>         filterOut_;
  std::optional<CSVGColorMatrixType> type_;
  ValueList                          values_;
};

#endif
