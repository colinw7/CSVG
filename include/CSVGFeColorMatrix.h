#ifndef CSVGFeColorMatrix_H
#define CSVGFeColorMatrix_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeColorMatrix : public CSVGFilterBase {
 public:
  typedef std::vector<double> ValueList;

 public:
  CSVG_OBJECT_DEF("feColorMatrix", CSVGObjTypeId::FE_COLOR_MATRIX)

  CSVGFeColorMatrix(CSVG &svg);
  CSVGFeColorMatrix(const CSVGFeColorMatrix &blur);

  CSVGFeColorMatrix *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("SourceGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("SourceGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  CSVGColorMatrixType getType() const { return type_.getValue(CSVGColorMatrixType::MATRIX); }
  void setType(CSVGColorMatrixType t) { type_ = t; }

  const ValueList &getValues() const { return values_; }
  void setValues(const ValueList &values) { values_ = values; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeColorMatrix &filter);

 private:
  COptValT<std::string>         filterIn_;
  COptValT<std::string>         filterOut_;
  COptValT<CSVGColorMatrixType> type_;
  ValueList                     values_;
};

#endif
