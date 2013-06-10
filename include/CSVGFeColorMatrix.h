#include <CSVGFilter.h>

class CSVGFeColorMatrix : public CSVGFilter {
 private:
  enum Type {
    MATRIX_TYPE,
    SATURATE_TYPE,
    HUE_ROTATE_TYPE,
    LUMINANCE_TO_ALPHA_TYPE
  };

  typedef std::vector<double> ValueList;

  std::string filter_in_;
  std::string filter_out_;
  Type        type_;
  ValueList   values_;

 public:
  CSVG_OBJECT_DEF("feColorMatrix", CSVG_OBJ_TYPE_FE_COLOR_MATRIX)

  CSVGFeColorMatrix(CSVG &svg);
  CSVGFeColorMatrix(const CSVGFeColorMatrix &blur);

  CSVGFeColorMatrix *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeColorMatrix &filter);
};
