#ifndef CSVGFeOffset_H
#define CSVGFeOffset_H

#include <CSVGFilter.h>

class CSVGFeOffset : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feOffset", CSVGObjTypeId::FE_OFFSET)

  CSVGFeOffset(CSVG &svg);
  CSVGFeOffset(const CSVGFeOffset &offset);

  CSVGFeOffset *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeOffset &filter);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
  COptValT<double>      dx_;
  COptValT<double>      dy_;
};

#endif
