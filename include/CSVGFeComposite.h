#ifndef CSVGFeComposite_H
#define CSVGFeComposite_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeComposite : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feComposite", CSVGObjTypeId::FE_COMPOSITE)

  CSVGFeComposite(CSVG &svg);
  CSVGFeComposite(const CSVGFeComposite &fe);

  CSVGFeComposite *dup() const override;

  CRGBACombineFunc getType() const { return type_.getValue(CRGBA_COMBINE_OVER); }
  void setType(CRGBACombineFunc t) { type_ = t; }

  std::string getFilterIn1() const { return filterIn1_.getValue("FilterGraphic"); }
  void setFilterIn1(const std::string &s) { filterIn1_ = s; }

  std::string getFilterIn2() const { return filterIn2_.getValue("FilterGraphic"); }
  void setFilterIn2(const std::string &s) { filterIn2_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("FilterGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  double getK1() const { return k1_.getValue(0); }
  void setK1(double r) { k1_ = r; }

  double getK2() const { return k2_.getValue(0); }
  void setK2(double r) { k2_ = r; }

  double getK3() const { return k3_.getValue(0); }
  void setK3(double r) { k3_ = r; }

  double getK4() const { return k4_.getValue(0); }
  void setK4(double r) { k4_ = r; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeComposite &fe);

 private:
  void filterImage(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, CSVGBuffer *outBuffer);

 private:
  COptValT<CRGBACombineFunc> type_;
  COptValT<std::string>      filterIn1_;
  COptValT<std::string>      filterIn2_;
  COptValT<std::string>      filterOut_;
  COptValT<double>           k1_, k2_, k3_, k4_;
};

#endif
