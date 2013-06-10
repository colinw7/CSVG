class CSVGStyleData;

class CSVGStyle : public CSVGObject {
 private:
  enum StyleType {
    STYLE_NONE,
    STYLE_TEXT_CSS
  };

 public:
  CSVG_OBJECT_DEF("style", CSVG_OBJ_TYPE_STYLE)

  CSVGStyle(CSVG &svg);
  CSVGStyle(const CSVGStyle &style);

  CSVGStyle *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void setText(const std::string &text);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGStyle &style);

 private:
  bool parseCSS(const std::string &str);

 private:
  StyleType type_;
};
