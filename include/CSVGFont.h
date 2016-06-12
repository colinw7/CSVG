#ifndef CSVGFont_H
#define CSVGFont_H

#include <CSVGObject.h>

class CSVGFontFace;
class CSVGMissingGlyph;
class CSVGGlyph;

class CSVGFont : public CSVGObject {
 public:
  typedef std::map<char       , CSVGGlyph *> CharGlyphMap;
  typedef std::map<std::string, CSVGGlyph *> UnicodeGlyphMap;

 public:
  CSVG_OBJECT_DEF("font", CSVGObjTypeId::FONT)

  CSVGFont(CSVG &svg);
  CSVGFont(const CSVGFont &font);

  CSVGFont *dup() const override;

  CSVGFontFace *getFontFace() const { return font_face_; }

  bool processOption(const std::string &name, const std::string &value) override;

  void termParse();

  bool draw() override;

  CSVGGlyph *getCharGlyph(char c) const;
  CSVGGlyph *getUnicodeGlyph(const std::string &unicode) const;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFont &font);

 private:
  CSVGFont &operator=(const CSVGFont &rhs);

 private:
  COptReal          hxo_;
  COptReal          hyo_;
  COptReal          hdx_;
  COptReal          vdy_;
  COptReal          vxo_;
  COptReal          vyo_;
  CSVGFontFace     *font_face_     { 0 };
  CSVGMissingGlyph *missing_glyph_ { 0 };
  CharGlyphMap      char_glyph_map_;
  UnicodeGlyphMap   unicode_glyph_map_;
};

#endif
