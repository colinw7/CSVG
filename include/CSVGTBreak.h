#ifndef CSVGTBreak_H
#define CSVGTBreak_H

#include <CSVGObject.h>

class CSVGTBreak : public CSVGObject, public CSVGPrintBase<CSVGTBreak> {
 public:
  CSVG_OBJECT_DEF("tbreak", CSVGObjTypeId::TBREAK)

  CSVGTBreak(CSVG &svg);
  CSVGTBreak(const CSVGTBreak &tbreak);

  CSVGTBreak *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  COptString getNameValue(const std::string &name) const override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }
};

#endif
