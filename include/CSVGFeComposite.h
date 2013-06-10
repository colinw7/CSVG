#include <CSVGFilter.h>

class CSVGFeComposite : public CSVGFilter {
 private:
  CRGBACombineFunc type_;
  std::string      filter_in1_;
  std::string      filter_in2_;
  std::string      filter_out_;

 public:
  CSVG_OBJECT_DEF("feComposite", CSVG_OBJ_TYPE_FE_COMPOSITE)

  CSVGFeComposite(CSVG &svg);
  CSVGFeComposite(const CSVGFeComposite &fe);

  CSVGFeComposite *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage2(CImagePtr src_image1, CImagePtr src_image2);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeComposite &fe);
};
