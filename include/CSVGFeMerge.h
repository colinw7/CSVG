#ifndef CSVGFeMerge_H
#define CSVGFeMerge_H

#include <CSVGFilter.h>

class CSVGFeMerge : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feMerge", CSVGObjTypeId::FE_MERGE)

  CSVGFeMerge(CSVG &svg);
  CSVGFeMerge(const CSVGFeMerge &merge);

  CSVGFeMerge *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeMerge &filter);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
};

#endif
