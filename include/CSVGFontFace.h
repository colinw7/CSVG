#ifndef CSVGFontFace_H
#define CSVGFontFace_H

#include <CSVGObject.h>

class CSVGFontFace : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("font-face", CSVGObjTypeId::FONT_FACE)

  CSVGFontFace(CSVG &svg);
  CSVGFontFace(const CSVGFontFace &font_face);

  CSVGFontFace *dup() const override;

  int getUnits() const { return units_.getValue(0); }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFontFace &face);

 private:
  COptValT<std::string> family_;
  COptValT<int>         units_;
  COptValT<std::string> panose_;
  COptValT<int>         ascent_;
  COptValT<int>         descent_;
  COptValT<bool>        alphabetic_;
};

#endif
