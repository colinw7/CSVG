#ifndef CSVGFontFace_H
#define CSVGFontFace_H

#include <CSVGObject.h>

class CSVGFontFace : public CSVGObject, public CSVGPrintBase<CSVGFontFace> {
 public:
  CSVG_OBJECT_DEF("font-face", CSVGObjTypeId::FONT_FACE)

  CSVGFontFace(CSVG &svg);
  CSVGFontFace(const CSVGFontFace &font_face);

  CSVGFontFace *dup() const override;

  int getUnits() const { return unitsPerEm_.value_or(1000); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<std::string> fontFamily_;
  std::optional<std::string> fontStyle_;
  std::optional<std::string> fontVariant_;
  std::optional<std::string> fontWeight_;
  std::optional<std::string> fontStretch_;
  std::optional<std::string> fontSize_;
  std::optional<std::string> unicodeRange_;
  std::optional<int>         unitsPerEm_;
  std::optional<std::string> panose_;
  std::optional<std::string> stemv_;
  std::optional<std::string> stemh_;
  std::optional<std::string> slope_;
  std::optional<std::string> capHeight_;
  std::optional<std::string> xheight_;
  std::optional<std::string> accentHeight_;
  std::optional<int>         ascent_;
  std::optional<int>         descent_;
  std::optional<int>         baseline_;
  std::optional<std::string> widths_;
  std::optional<std::string> bbox_;
  std::optional<std::string> ideographic_;
  std::optional<bool>        alphabetic_;
  std::optional<std::string> mathematical_;
  std::optional<std::string> hanging_;
  std::optional<std::string> vIdeographic_;
  std::optional<std::string> vAlphabetic_;
  std::optional<std::string> vMathematical_;
  std::optional<std::string> vHanging_;
  std::optional<std::string> underlinePosition_;
  std::optional<std::string> underlineThickness_;
  std::optional<std::string> strikethroughPosition_;
  std::optional<std::string> strikethroughThickness_;
  std::optional<std::string> overlinePosition_;
  std::optional<std::string> overlineThickness_;
};

#endif
