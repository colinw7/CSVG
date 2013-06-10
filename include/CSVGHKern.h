class CSVGHKern : public CSVGObject {
 private:
  std::string glyph1_;
  std::string glyph2_;
  int         k_;

 public:
  CSVG_OBJECT_DEF("hkern", CSVG_OBJ_TYPE_HKERN)

  CSVGHKern(CSVG &svg);
  CSVGHKern(const CSVGHKern &hk);

  CSVGHKern *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  bool isDrawable() { return false; }

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGHKern &hkern);
};
