#include <CSVGFilter.h>

class CSVGFeBlend : public CSVGFilter {
 public:
  enum Type {
    NORMAL,
    MULTIPLY,
    SCREEN,
    DARKEN,
    LIGHTEN
  };

 private:
  Type        type_;
  std::string filter_in1_;
  std::string filter_in2_;
  std::string filter_out_;

 public:
  CSVG_OBJECT_DEF("feBlend", CSVG_OBJ_TYPE_FE_BLEND)

  CSVGFeBlend(CSVG &svg);
  CSVGFeBlend(const CSVGFeBlend &fe);

  CSVGFeBlend *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage2(CImagePtr src_image1, CImagePtr src_image2);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeBlend &fe);
};
