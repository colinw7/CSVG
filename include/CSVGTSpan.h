class CSVGTSpan : public CSVGObject {
 private:
  CPoint2D     position_;
  std::string  text_;
  CSVGFontDef *font_;

 public:
  CSVG_OBJECT_DEF("tspan", CSVG_OBJ_TYPE_TSPAN)

  CSVGTSpan(CSVG &svg);
  CSVGTSpan(const CSVGTSpan &span);

  CSVGTSpan *dup() const;

  const std::string &getText() const { return text_; }

  void setX(double x) { position_.x = x; }
  void setY(double y) { position_.y = y; }

  void setText(const std::string &text) { text_ = text; }

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGTSpan &tspan);

 private:
  CSVGTSpan &operator=(const CSVGTSpan &rhs);
};
