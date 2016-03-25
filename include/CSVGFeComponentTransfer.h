#ifndef CSVGFeComponentTransfer_H
#define CSVGFeComponentTransfer_H

#include <CSVGFilter.h>

class CSVGFeComponentTransfer : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feComponentTransfer", CSVGObjTypeId::FE_COMPONENT_TRANSFER)

  CSVGFeComponentTransfer(CSVG &svg);
  CSVGFeComponentTransfer(const CSVGFeComponentTransfer &fe);

  CSVGFeComponentTransfer *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeComponentTransfer &fe);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
};

#endif
