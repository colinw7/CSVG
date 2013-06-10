#include <CSVGFilter.h>

class CSVGFeFlood : public CSVGFilter {
 private:
  std::string filter_in_;
  std::string filter_out_;
  CRGBA       color_;

 public:
  CSVG_OBJECT_DEF("feFlood", CSVG_OBJ_TYPE_FE_FLOOD)

  CSVGFeFlood(CSVG &svg);
  CSVGFeFlood(const CSVGFeFlood &fe);

  CSVGFeFlood *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeFlood &fe);
};
