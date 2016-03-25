#ifndef CSVGFeComposite_H
#define CSVGFeComposite_H

#include <CSVGFilter.h>

class CSVGFeComposite : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feComposite", CSVGObjTypeId::FE_COMPOSITE)

  CSVGFeComposite(CSVG &svg);
  CSVGFeComposite(const CSVGFeComposite &fe);

  CSVGFeComposite *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage2(CImagePtr src_image1, CImagePtr src_image2);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeComposite &fe);

 private:
  COptValT<CRGBACombineFunc> type_;
  COptValT<std::string>      filter_in1_;
  COptValT<std::string>      filter_in2_;
  COptValT<std::string>      filter_out_;
  COptValT<double>           k1_, k2_, k3_, k4_;
};

#endif
