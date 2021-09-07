#ifndef CSVGMetaData_H
#define CSVGMetaData_H

#include <CSVGObject.h>

class CSVGMetaData : public CSVGObject, public CSVGPrintBase<CSVGMetaData> {
 public:
  CSVG_OBJECT_DEF("metadata", CSVGObjTypeId::METADATA)

  CSVGMetaData(CSVG &svg);

  CSVGMetaData *dup() const override { return new CSVGMetaData(svg_); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isHierDrawable() const override { return false; }

  bool getBBox(CBBox2D &) const override { return false; }

  bool draw() override { return false; }

  void print(std::ostream &os, bool hier=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }
};

#endif
