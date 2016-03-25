#ifndef CSVGFeBlend_H
#define CSVGFeBlend_H

#include <CSVGFilter.h>

class CSVGFeBlend : public CSVGFilter {
 public:
  enum class Type {
    NORMAL,
    MULTIPLY,
    SCREEN,
    DARKEN,
    LIGHTEN
  };

 public:
  CSVG_OBJECT_DEF("feBlend", CSVGObjTypeId::FE_BLEND)

  CSVGFeBlend(CSVG &svg);
  CSVGFeBlend(const CSVGFeBlend &fe);

  CSVGFeBlend *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage2(CImagePtr src_image1, CImagePtr src_image2);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeBlend &fe);

 private:
  COptValT<Type>        type_;
  COptValT<std::string> filter_in1_;
  COptValT<std::string> filter_in2_;
  COptValT<std::string> filter_out_;
};

#endif
