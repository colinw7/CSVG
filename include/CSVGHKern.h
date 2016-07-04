#ifndef CSVGHKern_H
#define CSVGHKern_H

#include <CSVGObject.h>

class CSVGHKern : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("hkern", CSVGObjTypeId::HKERN)

  CSVGHKern(CSVG &svg);
  CSVGHKern(const CSVGHKern &hk);

  CSVGHKern *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGHKern &hkern);

 private:
  COptString glyph1_;
  COptString glyph2_;
  COptInt    k_;
};

#endif
