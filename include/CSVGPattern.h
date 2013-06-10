class CSVGPattern : public CSVGObject {
 private:
  CSVGObject     *object_;
  double          x_, y_;
  double          width_, height_;
  CSVGCoordUnits  units_, content_units_;

 public:
  CSVG_OBJECT_DEF("pattern", CSVG_OBJ_TYPE_PATTERN)

  CSVGPattern(CSVG &svg);
  CSVGPattern(const CSVGPattern &pattern);

  CSVGPattern *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  void fillRectangle(double x1, double y1, double x2, double y2);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPattern &pattern);

  CImagePtr getImage(double w, double h, double *w1, double *h1);

 private:
  CSVGPattern &operator=(const CSVGPattern &rhs);
};
