class CSVGCircle : public CSVGObject {
 private:
  CPoint2D center_;
  double   radius_;

 public:
  CSVG_OBJECT_DEF("circle", CSVG_OBJ_TYPE_CIRCLE)

  CSVGCircle(CSVG &svg);
  CSVGCircle(const CSVGCircle &circle);

  CSVGCircle *dup() const;

  const CPoint2D &getCenter() const { return center_; }

  double getRadius() const { return radius_; }

  void setCenter(const CPoint2D &center) { center_ = center; }

  void setCenterX(double x) { center_.x = x; }
  void setCenterY(double y) { center_.y = y; }

  void setRadius(double r) { radius_ = r; }

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void moveBy(const CVector2D &delta);

  void resizeTo(const CSize2D &size);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGCircle &circle);
};
