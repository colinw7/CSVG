class CSVGFontFace;
class CSVGMissingGlyph;
class CSVGGlyph;

class CSVGFont : public CSVGObject {
 public:
  typedef std::map<char       , CSVGGlyph *> CharGlyphMap;
  typedef std::map<std::string, CSVGGlyph *> UnicodeGlyphMap;

 private:
  int               hxo_, hyo_, hdx_;
  int               vxo_, vyo_, vdy_;
  CSVGFontFace     *font_face_;
  CSVGMissingGlyph *missing_glyph_;
  CharGlyphMap      char_glyph_map_;
  UnicodeGlyphMap   unicode_glyph_map_;

 public:
  CSVG_OBJECT_DEF("font", CSVG_OBJ_TYPE_FONT)

  CSVGFont(CSVG &svg);
  CSVGFont(const CSVGFont &font);

  CSVGFont *dup() const;

  CSVGFontFace *getFontFace() const { return font_face_; }

  bool processOption(const std::string &name, const std::string &value);

  void termParse();

  void draw();

  CSVGGlyph *getCharGlyph(char c) const;
  CSVGGlyph *getUnicodeGlyph(const std::string &unicode) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFont &font);

 private:
  CSVGFont &operator=(const CSVGFont &rhs);
};
