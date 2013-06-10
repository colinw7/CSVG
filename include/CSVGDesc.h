class CSVGDesc : public CSVGObject {
 private:
  std::string text_;

 public:
  CSVG_OBJECT_DEF("desc", CSVG_OBJ_TYPE_DESC)

  CSVGDesc(CSVG &svg);
  CSVGDesc(const CSVGDesc &desc);

  CSVGDesc *dup() const;

  void setText(const std::string &text) { text_ = text; }

  bool processOption(const std::string &name, const std::string &value);

  bool isDrawable() { return false; }

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGDesc &desc);
};
