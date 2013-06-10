class CSVGMissingGlyph : public CSVGGlyph {
 public:
  CSVG_OBJECT_DEF("missing-glyph", CSVG_OBJ_TYPE_MISSING_GLYPH)

  CSVGMissingGlyph(CSVG &svg);

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMissingGlyph &glyph);
};
