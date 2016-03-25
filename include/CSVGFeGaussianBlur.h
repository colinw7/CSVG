#ifndef CSVGFeGaussianBlur_H
#define CSVGFeGaussianBlur_H

#include <CSVGFilter.h>

class CSVGFeGaussianBlur : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feGaussianBlur", CSVGObjTypeId::FE_GAUSSIAN_BLUR)

  CSVGFeGaussianBlur(CSVG &svg);
  CSVGFeGaussianBlur(const CSVGFeGaussianBlur &blur);

  CSVGFeGaussianBlur *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeGaussianBlur &filter);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
  COptValT<double>      std_dev_;
};

#endif
