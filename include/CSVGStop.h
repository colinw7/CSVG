class CSVGStop : public CSVGObject {
 private:
  double offset_;
  CRGBA  color_;
  double opacity_;

 public:
  CSVG_OBJECT_DEF("stop", CSVG_OBJ_TYPE_STOP)

  CSVGStop(CSVG &svg);
  CSVGStop(const CSVGStop &stop);

  CSVGStop *dup() const;

  double getOffset () const { return offset_ ; }
  CRGBA  getColor  () const { return color_  ; }
  double getOpacity() const { return opacity_; }

  CRGBA getAlphaColor() const;

  void setOffset (double offset     ) { offset_  = offset ; }
  void setColor  (const CRGBA &color) { color_   = color  ; }
  void setOpacity(double opacity    ) { opacity_ = opacity; }

  bool processOption(const std::string &name, const std::string &value);

  bool isDrawable() { return false; }

  void draw();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGStop &stop);
};
