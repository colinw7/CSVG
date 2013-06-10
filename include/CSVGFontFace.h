class CSVGFontFace : public CSVGObject {
 private:
  std::string family_;
  int         units_;
  std::string panose_;
  int         ascent_;
  int         descent_;
  bool        alphabetic_;

 public:
  CSVG_OBJECT_DEF("font-face", CSVG_OBJ_TYPE_FONT_FACE)

  CSVGFontFace(CSVG &svg);
  CSVGFontFace(const CSVGFontFace &font_face);

  CSVGFontFace *dup() const;

  int getUnits() const { return units_; }

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFontFace &face);
};
