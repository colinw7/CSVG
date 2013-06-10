class CSVGText : public CSVGObject {
 private:
  CPoint2D    position_;
  std::string text_;

 public:
  CSVG_OBJECT_DEF("text", CSVG_OBJ_TYPE_TEXT)

  CSVGText(CSVG &svg);
  CSVGText(const CSVGText &text);

  CSVGText *dup() const;

  const CPoint2D &getPosition() const { return position_; }

  const std::string &getText() const { return text_; }

  void setX(double x) { position_.x = x; }
  void setY(double y) { position_.y = y; }

  void setText(const std::string &text) { text_ = text; }

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGText &text);
};
