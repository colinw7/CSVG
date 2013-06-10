class CSVGRect : public CSVGObject {
 private:
  CBBox2D bbox_;
  double  x_, y_, width_, height_;
  double  rx_, ry_;

 public:
  CSVG_OBJECT_DEF("rect", CSVG_OBJ_TYPE_RECT)

  CSVGRect(CSVG &svg);
  CSVGRect(const CSVGRect &rect);

  CSVGRect *dup() const;

  const CBBox2D &getBBox() { return bbox_; }

  double getX() { return x_; }
  double getY() { return y_; }

  double getWidth () { return width_; }
  double getHeight() { return height_; }

  double getRX() { return rx_; }
  double getRY() { return ry_; }

  void setOrigin(const CPoint2D &point) {
    bbox_.setLL(point);
  }

  void setSize(const CSize2D &size) {
    bbox_.setSize(size);
  }

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void moveBy(const CVector2D &delta);
  void resizeTo(const CSize2D &size);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGRect &rect);
};
