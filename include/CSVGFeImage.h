#ifndef CSVGFeImage_H
#define CSVGFeImage_H

#include <CSVGFilter.h>
#include <CSVGXLink.h>

class CSVGFeImage : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feImage", CSVGObjTypeId::FE_IMAGE)

  CSVGFeImage(CSVG &svg);
  CSVGFeImage(const CSVGFeImage &image);

  CSVGFeImage *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeImage &filter);

 private:
  CSVGXLink             xlink_;
  COptValT<std::string> filter_out_;
};

#endif
