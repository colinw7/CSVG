#ifndef CSVGFeSpecularLighting_H
#define CSVGFeSpecularLighting_H

#include <CSVGFilter.h>

class CSVGFeSpecularLighting : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feSpecularLighting", CSVGObjTypeId::FE_SPECULAR_LIGHTING)

  CSVGFeSpecularLighting(CSVG &svg);
  CSVGFeSpecularLighting(const CSVGFeSpecularLighting &fe);

  CSVGFeSpecularLighting *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeSpecularLighting &fe);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
};

#endif
