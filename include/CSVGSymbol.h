#ifndef CSVGSymbol_H
#define CSVGSymbol_H

#include <CSVGObject.h>

class CSVGSymbol : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("symbol", CSVGObjTypeId::SYMBOL)

  CSVGSymbol(CSVG &svg);
  CSVGSymbol(const CSVGSymbol &symbol);

  CSVGSymbol *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void moveBy(const CVector2D &delta) override;
  void resizeTo(const CSize2D &size) override;
  void rotateBy(double da, const CPoint2D &c) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGSymbol &group);
};

#endif
