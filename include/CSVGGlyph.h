class CSVGPathPart;

class CSVGGlyph : public CSVGObject {
 public:
  typedef std::vector<CSVGPathPart *> PartList;

 protected:
  std::string unicode_;
  std::string name_;
  int         dx_;
  PartList    parts_;

 public:
  CSVG_OBJECT_DEF("glyph", CSVG_OBJ_TYPE_GLYPH)

  CSVGGlyph(CSVG &svg);
  CSVGGlyph(const CSVGGlyph &glyph);

  virtual ~CSVGGlyph() { }

  CSVGGlyph *dup() const;

  std::string getUnicode() const { return unicode_; }
  std::string getName   () const { return name_   ; }
  int         getDx     () const { return dx_     ; }

  virtual bool processOption(const std::string &name, const std::string &value);

  virtual void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGGlyph &glyph);
};
