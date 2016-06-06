#ifndef CSVGDefs_H
#define CSVGDefs_H

#include <CSVGObject.h>

class CXMLToken;

class CSVGDefs : public CSVGObject {
 public:
  typedef std::vector<const CXMLToken *> TokenList;

 public:
  CSVG_OBJECT_DEF("defs", CSVGObjTypeId::DEFS)

  CSVGDefs(CSVG &svg);
  CSVGDefs(const CSVGDefs &defs);

  CSVGDefs *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool isHierDrawable() const override { return false; }

  bool getBBox(CBBox2D &) const override { return false; }

  bool draw() override { return false; }

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGDefs &defs);

 private:
  TokenList tokens_;
};

#endif
