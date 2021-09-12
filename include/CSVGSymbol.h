#ifndef CSVGSymbol_H
#define CSVGSymbol_H

#include <CSVGObject.h>

class CSVGSymbol : public CSVGObject, public CSVGPrintBase<CSVGSymbol> {
 public:
  CSVG_OBJECT_DEF("symbol", CSVGObjTypeId::SYMBOL)

  CSVGSymbol(CSVG &svg);
  CSVGSymbol(const CSVGSymbol &symbol);

  CSVGSymbol *dup() const override;

  CSVGPreserveAspect preserveAspect() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  bool processOption(const std::string &name, const std::string &value) override;

  void resizeTo(const CSize2D &size) override;

  void rotateBy(double da) override;

  bool canFlatten() const override { return false; }

  bool isDrawable() const override { return false; }

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  void moveDelta(const CVector2D &delta) override;

 private:
  COptValT<CSVGPreserveAspect> preserveAspect_;
};

#endif
