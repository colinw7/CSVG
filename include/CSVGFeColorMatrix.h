#ifndef CSVGFeColorMatrix_H
#define CSVGFeColorMatrix_H

#include <CSVGFilter.h>

class CSVGFeColorMatrix : public CSVGFilter {
 public:
  enum class Type {
    MATRIX_TYPE,
    SATURATE_TYPE,
    HUE_ROTATE_TYPE,
    LUMINANCE_TO_ALPHA_TYPE
  };

 public:
  CSVG_OBJECT_DEF("feColorMatrix", CSVGObjTypeId::FE_COLOR_MATRIX)

  CSVGFeColorMatrix(CSVG &svg);
  CSVGFeColorMatrix(const CSVGFeColorMatrix &blur);

  CSVGFeColorMatrix *dup() const override;

  Type getType() const { return type_.getValue(Type::MATRIX_TYPE); }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeColorMatrix &filter);

 private:
  typedef std::vector<double> ValueList;

  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
  COptValT<Type>        type_;
  ValueList             values_;
};

#endif
