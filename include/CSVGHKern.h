#ifndef CSVGHKern_H
#define CSVGHKern_H

#include <CSVGObject.h>

class CSVGHKern : public CSVGObject, public CSVGPrintBase<CSVGHKern> {
 public:
  CSVG_OBJECT_DEF("hkern", CSVGObjTypeId::HKERN)

  CSVGHKern(CSVG &svg);
  CSVGHKern(const CSVGHKern &hk);

  CSVGHKern *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptString glyph1_;
  COptString glyph2_;
  COptInt    k_;
};

#endif
