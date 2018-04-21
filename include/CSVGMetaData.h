#ifndef CSVGMetaData_H
#define CSVGMetaData_H

#include <CSVGObject.h>

class CSVGMetaData : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("metadata", CSVGObjTypeId::METADATA)

  CSVGMetaData(CSVG &svg);

  CSVGMetaData *dup() const override { return new CSVGMetaData(svg_); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMetaData &glyph);
};

#endif
