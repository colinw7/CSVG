#ifndef CSVGFeFlood_H
#define CSVGFeFlood_H

#include <CSVGFilter.h>

class CSVGFeFlood : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feFlood", CSVGObjTypeId::FE_FLOOD)

  CSVGFeFlood(CSVG &svg);
  CSVGFeFlood(const CSVGFeFlood &fe);

  CSVGFeFlood *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeFlood &fe);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
  COptValT<CRGBA>       color_;
};

#endif
