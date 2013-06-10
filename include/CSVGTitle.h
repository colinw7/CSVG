class CSVGTitle : public CSVGObject {
 private:
  std::string text_;

 public:
  CSVG_OBJECT_DEF("title", CSVG_OBJ_TYPE_TITLE)

  CSVGTitle(CSVG &svg);
  CSVGTitle(const CSVGTitle &title);

  CSVGTitle *dup() const;

  void setText(const std::string &text) { text_ = text; }

  std::string getText() const { return text_; }

  bool processOption(const std::string &name, const std::string &value);

  void draw();
};
