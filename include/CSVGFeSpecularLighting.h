#ifndef CSVGFeSpecularLighting_H
#define CSVGFeSpecularLighting_H

#include <CSVGFeLighting.h>

class CSVGFeSpecularLighting : public CSVGFeLighting {
 public:
  CSVG_OBJECT_DEF("feSpecularLighting", CSVGObjTypeId::FE_SPECULAR_LIGHTING)

  CSVGFeSpecularLighting(CSVG &svg);
  CSVGFeSpecularLighting(const CSVGFeSpecularLighting &fe);

  CSVGFeSpecularLighting *dup() const override;

  bool isSpecular() const override { return true; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeSpecularLighting &fe);
};

#endif
