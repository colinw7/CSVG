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

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGHKern &hkern);

 private:
  COptValT<std::string> glyph1_;
  COptValT<std::string> glyph2_;
  COptValT<int>         k_;
};

#endif
