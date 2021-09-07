#ifndef CSVGDefs_H
#define CSVGDefs_H

#include <CSVGObject.h>

class CXMLToken;

class CSVGDefs : public CSVGObject, public CSVGPrintBase<CSVGDefs> {
 public:
  using TokenList = std::vector<const CXMLToken *>;

 public:
  CSVG_OBJECT_DEF("defs", CSVGObjTypeId::DEFS)

  CSVGDefs(CSVG &svg);
  CSVGDefs(const CSVGDefs &defs);

  CSVGDefs *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool isHierDrawable() const override { return false; }

  bool getBBox(CBBox2D &) const override { return false; }

  bool draw() override { return false; }

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  TokenList tokens_;
};

#endif
