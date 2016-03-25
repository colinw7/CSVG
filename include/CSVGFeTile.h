#ifndef CSVGFeTile_H
#define CSVGFeTile_H

#include <CSVGFilter.h>

class CSVGFeTile : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feTile", CSVGObjTypeId::FE_TILE)

  CSVGFeTile(CSVG &svg);
  CSVGFeTile(const CSVGFeTile &fe);

  CSVGFeTile *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeTile &fe);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
};

#endif
