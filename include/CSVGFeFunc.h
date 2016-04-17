#ifndef CSVGFeFunc_H
#define CSVGFeFunc_H

#include <CSVGObject.h>

class CSVGFeFunc : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("feFunc", CSVGObjTypeId::FE_FUNC)

  CSVGFeFunc(CSVG &svg, CColorComponent component);
  CSVGFeFunc(const CSVGFeFunc &func);

  CSVGFeFunc *dup() const override;

  CSVGFilterFuncType getType() const { return type_.getValue(CSVGFilterFuncType::LINEAR); }

  double getSlope() const { return slope_.getValue(1); }

  double getIntercept() const { return intercept_.getValue(0); }

  double getAmplitude() const { return amplitude_.getValue(1); }

  double getExponent() const { return exponent_.getValue(1); }

  double getOffset() const { return offset_.getValue(0); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeFunc &filter);

 private:
  typedef std::vector<double> ValueTable;

  CColorComponent              component_;
  COptValT<CSVGFilterFuncType> type_;
  // linear
  COptValT<double>             slope_;
  COptValT<double>             intercept_;
  // gamma
  COptValT<double>             amplitude_;
  COptValT<double>             exponent_;
  COptValT<double>             offset_;
  // table/discrete
  ValueTable                   table_;
};

#endif
