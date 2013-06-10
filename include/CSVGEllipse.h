class CSVGEllipse : public CSVGObject {
 private:
  CPoint2D center_;
  double   rx_, ry_;

 public:
  CSVG_OBJECT_DEF("ellipse", CSVG_OBJ_TYPE_ELLIPSE)

  CSVGEllipse(CSVG &svg);
  CSVGEllipse(const CSVGEllipse &ellipse);

  CSVGEllipse *dup() const;

  const CPoint2D &getCenter() const { return center_; }

  double getRX() const { return rx_; }
  double getRY() const { return ry_; }

  void setCenter(const CPoint2D &center) { center_ = center; }

  void setCenterX(double x) { center_.x = x; }
  void setCenterY(double y) { center_.y = y; }

  void setRadius(double rx, double ry) { rx_ = rx; ry_ = ry; }

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void moveBy(const CVector2D &delta);

  void resizeTo(const CSize2D &size);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGEllipse &ellipse);
};
