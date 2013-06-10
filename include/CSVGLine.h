class CSVGLine : public CSVGObject {
 private:
  CPoint2D point1_;
  CPoint2D point2_;

 public:
  CSVG_OBJECT_DEF("line", CSVG_OBJ_TYPE_LINE)

  CSVGLine(CSVG &svg);
  CSVGLine(const CSVGLine &line);

  CSVGLine *dup() const;

  const CPoint2D &getStart() const { return point1_; }
  const CPoint2D &getEnd  () const { return point2_; }

  void setStart(const CPoint2D &start) { point1_ = start; }
  void setEnd  (const CPoint2D &end  ) { point2_ = end  ; }

  void setX1(double x1) { point1_.x = x1; }
  void setY1(double y1) { point1_.y = y1; }
  void setX2(double x2) { point1_.x = x2; }
  void setY2(double y2) { point1_.y = y2; }

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void moveBy(const CVector2D &delta);
  void resizeTo(const CSize2D &size);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGLine &line);
};
