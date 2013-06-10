#include <CSVGFilter.h>

class CSVGFePointLight : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("fePointLight", CSVG_OBJ_TYPE_FE_POINT_LIGHT)

  CSVGFePointLight(CSVG &svg);
  CSVGFePointLight(const CSVGFePointLight &fe);

  CSVGFePointLight *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFePointLight &fe);
};
