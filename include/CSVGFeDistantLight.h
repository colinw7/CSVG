#include <CSVGFilter.h>

class CSVGFeDistantLight : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feDistantLight", CSVG_OBJ_TYPE_FE_DISTANT_LIGHT)

  CSVGFeDistantLight(CSVG &svg);
  CSVGFeDistantLight(const CSVGFeDistantLight &fe);

  CSVGFeDistantLight *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeDistantLight &fe);
};
