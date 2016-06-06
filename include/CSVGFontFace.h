#ifndef CSVGFontFace_H
#define CSVGFontFace_H

#include <CSVGObject.h>

class CSVGFontFace : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("font-face", CSVGObjTypeId::FONT_FACE)

  CSVGFontFace(CSVG &svg);
  CSVGFontFace(const CSVGFontFace &font_face);

  CSVGFontFace *dup() const override;

  int getUnits() const { return unitsPerEm_.getValue(0); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFontFace &face);

 private:
  COptString fontFamily_;
  COptString fontStyle_;
  COptString fontVariant_;
  COptString fontWeight_;
  COptString fontStretch_;
  COptString fontSize_;
  COptString unicodeRange_;
  COptInt    unitsPerEm_;
  COptString panose_;
  COptString stemv_;
  COptString stemh_;
  COptString slope_;
  COptString capHeight_;
  COptString xheight_;
  COptString accentHeight_;
  COptInt    ascent_;
  COptInt    descent_;
  COptString widths_;
  COptString bbox_;
  COptString ideographic_;
  COptBool   alphabetic_;
  COptString mathematical_;
  COptString hanging_;
  COptString vIdeographic_;
  COptString vAlphabetic_;
  COptString vMathematical_;
  COptString vHanging_;
  COptString underlinePosition_;
  COptString underlineThickness_;
  COptString strikethroughPosition_;
  COptString strikethroughThickness_;
  COptString overlinePosition_;
  COptString overlineThickness_;
};

#endif
