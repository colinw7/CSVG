#ifndef CSVGFeFunc_H
#define CSVGFeFunc_H

#include <CSVGObject.h>

class CSVGFeFunc : public CSVGObject {
 public:
  typedef std::vector<double> ValueTable;

 public:
  CSVG_OBJECT_DEF("feFunc", CSVGObjTypeId::FE_FUNC)

  CSVGFeFunc(CSVG &svg, CColorComponent component);
  CSVGFeFunc(const CSVGFeFunc &func);

  CSVGFeFunc *dup() const override;

  std::string getTagName() const override;

  CColorComponent getComponent() const { return component_; }
  void setComponent(CColorComponent c) { component_ = c; }

  CSVGFilterFuncType getType() const { return type_.getValue(CSVGFilterFuncType::LINEAR); }

  double getSlope() const { return slope_.getValue(1); }
  void setSlope(double r) { slope_ = r; }

  double getIntercept() const { return intercept_.getValue(0); }
  void setIntercept(double r) { intercept_ = r; }

  double getAmplitude() const { return amplitude_.getValue(1); }
  void setAmplitude(double r) { amplitude_ = r; }

  double getExponent() const { return exponent_.getValue(1); }
  void setExponent(double r) { exponent_ = r; }

  double getOffset() const { return offset_.getValue(0); }
  void setOffset(double r) { offset_ = r; }

  const ValueTable &getTable() const { return table_; }
  void setTable(const ValueTable &t) { table_ = t; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeFunc &filter);

 private:
  CColorComponent              component_;
  COptValT<CSVGFilterFuncType> type_;
  // linear
  COptReal                     slope_;
  COptReal                     intercept_;
  // gamma
  COptReal                     amplitude_;
  COptReal                     exponent_;
  COptReal                     offset_;
  // table/discrete
  ValueTable                   table_;
};

#endif
