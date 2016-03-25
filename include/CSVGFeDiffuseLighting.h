#ifndef CSVGFeDiffuseLighting_H
#define CSVGFeDiffuseLighting_H

#include <CSVGFilter.h>

class CSVGFeDiffuseLighting : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feDiffuseLighting", CSVGObjTypeId::FE_DIFFUSE_LIGHTING)

  CSVGFeDiffuseLighting(CSVG &svg);
  CSVGFeDiffuseLighting(const CSVGFeDiffuseLighting &fe);

  CSVGFeDiffuseLighting *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeDiffuseLighting &fe);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
};

#endif
