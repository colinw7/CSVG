#include <CSVGFilter.h>

class CSVGFeDiffuseLighting : public CSVGFilter {
 private:
  std::string filter_in_;
  std::string filter_out_;

 public:
  CSVG_OBJECT_DEF("feDiffuseLighting", CSVG_OBJ_TYPE_FE_DIFFUSE_LIGHTING)

  CSVGFeDiffuseLighting(CSVG &svg);
  CSVGFeDiffuseLighting(const CSVGFeDiffuseLighting &fe);

  CSVGFeDiffuseLighting *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeDiffuseLighting &fe);
};
