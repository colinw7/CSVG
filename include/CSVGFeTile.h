#include <CSVGFilter.h>

class CSVGFeTile : public CSVGFilter {
 private:
  std::string filter_in_;
  std::string filter_out_;

 public:
  CSVG_OBJECT_DEF("feTile", CSVG_OBJ_TYPE_FE_TILE)

  CSVGFeTile(CSVG &svg);
  CSVGFeTile(const CSVGFeTile &fe);

  CSVGFeTile *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeTile &fe);
};
