#include <CSVGFilter.h>

class CSVGFeGaussianBlur : public CSVGFilter {
 private:
  std::string filter_in_;
  std::string filter_out_;
  double      std_dev_;

 public:
  CSVG_OBJECT_DEF("feGaussianBlur", CSVG_OBJ_TYPE_FE_GAUSSIAN_BLUR)

  CSVGFeGaussianBlur(CSVG &svg);
  CSVGFeGaussianBlur(const CSVGFeGaussianBlur &blur);

  CSVGFeGaussianBlur *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeGaussianBlur &filter);
};
