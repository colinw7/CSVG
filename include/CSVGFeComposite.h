#ifndef CSVGFeComposite_H
#define CSVGFeComposite_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeComposite : public CSVGFilterBase, public CSVGPrintBase<CSVGFeComposite> {
 public:
  CSVG_OBJECT_DEF("feComposite", CSVGObjTypeId::FE_COMPOSITE)

  CSVGFeComposite(CSVG &svg);
  CSVGFeComposite(const CSVGFeComposite &fe);

  CSVGFeComposite *dup() const override;

  CRGBACombineFunc getType() const { return type_.value_or(CRGBA_COMBINE_OVER); }
  void setType(CRGBACombineFunc t) { type_ = t; }

  std::string getFilterIn1() const;
  void setFilterIn1(const std::string &s) { filterIn1_ = s; }

  std::string getFilterIn2() const;
  void setFilterIn2(const std::string &s) { filterIn2_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  double getK1() const { return k1_.value_or(0); }
  void setK1(double r) { k1_ = r; }

  double getK2() const { return k2_.value_or(0); }
  void setK2(double r) { k2_ = r; }

  double getK3() const { return k3_.value_or(0); }
  void setK3(double r) { k3_ = r; }

  double getK4() const { return k4_.value_or(0); }
  void setK4(double r) { k4_ = r; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<CRGBACombineFunc> type_;
  std::optional<std::string>      filterIn1_;
  std::optional<std::string>      filterIn2_;
  std::optional<std::string>      filterOut_;
  std::optional<double>           k1_, k2_, k3_, k4_;
};

#endif
