#ifndef CSVGFeImage_H
#define CSVGFeImage_H

#include <CSVGFilterBase.h>
#include <CSVGXLink.h>

class CSVGBuffer;

class CSVGFeImage : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feImage", CSVGObjTypeId::FE_IMAGE)

  CSVGFeImage(CSVG &svg);
  CSVGFeImage(const CSVGFeImage &image);

  CSVGFeImage *dup() const override;

  std::string getFilterOut() const { return filterOut_.getValue("SourceGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void filterImage(CImagePtr src_image, CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeImage &filter);

 private:
  COptValT<CSVGXLink>          xlink_;
  COptValT<std::string>        filterOut_;
  COptValT<CSVGPreserveAspect> preserveAspect_;
};

#endif
