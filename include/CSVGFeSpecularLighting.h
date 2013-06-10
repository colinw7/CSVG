#include <CSVGFilter.h>

class CSVGFeSpecularLighting : public CSVGFilter {
 private:
  std::string filter_in_;
  std::string filter_out_;

 public:
  CSVG_OBJECT_DEF("feSpecularLighting", CSVG_OBJ_TYPE_FE_SPECULAR_LIGHTING)

  CSVGFeSpecularLighting(CSVG &svg);
  CSVGFeSpecularLighting(const CSVGFeSpecularLighting &fe);

  CSVGFeSpecularLighting *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeSpecularLighting &fe);
};
