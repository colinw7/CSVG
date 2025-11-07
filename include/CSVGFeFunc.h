#ifndef CSVGFeFunc_H
#define CSVGFeFunc_H

#include <CSVGObject.h>

class CSVGFeFunc : public CSVGObject, public CSVGPrintBase<CSVGFeFunc> {
 public:
  using ValueTable = std::vector<double>;

 public:
  CSVG_OBJECT_DEF("feFunc", CSVGObjTypeId::FE_FUNC)

  CSVGFeFunc(CSVG &svg, CRGBAComponent component);
  CSVGFeFunc(const CSVGFeFunc &func);

  CSVGFeFunc *dup() const override;

  std::string getTagName() const override;

  CRGBAComponent getComponent() const { return component_; }
  void setComponent(CRGBAComponent c) { component_ = c; }

  CSVGFilterFuncType getType() const { return type_.value_or(CSVGFilterFuncType::LINEAR); }

  double getSlope() const { return slope_.value_or(1); }
  void setSlope(double r) { slope_ = r; }

  double getIntercept() const { return intercept_.value_or(0); }
  void setIntercept(double r) { intercept_ = r; }

  double getAmplitude() const { return amplitude_.value_or(1); }
  void setAmplitude(double r) { amplitude_ = r; }

  double getExponent() const { return exponent_.value_or(1); }
  void setExponent(double r) { exponent_ = r; }

  double getOffset() const { return offset_.value_or(0); }
  void setOffset(double r) { offset_ = r; }

  const ValueTable &getTable() const { return table_; }
  void setTable(const ValueTable &t) { table_ = t; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  CRGBAComponent                    component_;
  std::optional<CSVGFilterFuncType> type_;
  // linear
  std::optional<double>             slope_;
  std::optional<double>             intercept_;
  // gamma
  std::optional<double>             amplitude_;
  std::optional<double>             exponent_;
  std::optional<double>             offset_;
  // table/discrete
  ValueTable                        table_;
};

#endif
