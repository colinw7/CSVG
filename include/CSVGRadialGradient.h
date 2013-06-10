class CSVGStop;

class CSVGRadialGradient : public CSVGObject {
 public:
  typedef std::vector<CSVGStop *> StopList;

 private:
  COptValT<CPoint2D>            center_;
  COptValT<double>              radius_;
  COptValT<CPoint2D>            focus_;
  StopList                      stops_;
  COptValT<CMatrix2D>           gtransform_;
  COptValT<CSVGCoordUnits>      units_;
  COptValT<CGradientSpreadType> spread_;

 public:
  CSVG_OBJECT_DEF("radial_gradient", CSVG_OBJ_TYPE_RADIAL_GRADIENT)

  CSVGRadialGradient(CSVG &svg);
  CSVGRadialGradient(const CSVGRadialGradient &rg);

  CSVGRadialGradient *dup() const;

  const CPoint2D &getCenter() { return center_.getValue(); }

  double getCenterX() { return (center_.isValid() ? getCenter().x : 0.5); }
  double getCenterY() { return (center_.isValid() ? getCenter().y : 0.5); }

  void setCenter(const CPoint2D &center) { center_ = center; }

  void setCenterX(double x) {
    CPoint2D center(x, 0);

    if (center_.isValid())
      center.y = getCenterY();

    setCenter(center);
  }

  void setCenterY(double y) {
    CPoint2D center(0, y);

    if (center_.isValid())
      center.x = getCenterX();

    setCenter(center);
  }

  double getRadius() const { return (radius_.isValid() ? radius_.getValue() : 0.5); }

  void setRadius(double r) { radius_ = r; }

  const CPoint2D &getFocus () const { return focus_.getValue(); }

  double getFocusX() { return (focus_.isValid() ? getFocus().x : getCenterX()); }
  double getFocusY() { return (focus_.isValid() ? getFocus().y : getCenterY()); }

  void setFocus(const CPoint2D &focus) { focus_ = focus; }

  void setFocusX(double x) {
    CPoint2D focus(x, 0);

    if (focus_.isValid())
      focus.y = getFocusY();

    setFocus(focus);
  }

  void setFocusY(double y) {
    CPoint2D focus(0, y);

    if (focus_.isValid())
      focus.x = getFocusX();

    setFocus(focus);
  }

  bool getGTransformValid() const { return gtransform_.isValid(); }

  CMatrix2D getGTransform() const { return gtransform_.getValue(); }

  void setGTransform(const CMatrix2D &gtransform) { gtransform_ = gtransform; }

  bool getUnitsValid() const { return units_.isValid(); }

  CSVGCoordUnits getUnits() const { return units_.getValue(); }

  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getSpreadValid() const { return spread_.isValid(); }

  CGradientSpreadType getSpread() const { return spread_.getValue(); }

  void setSpread(CGradientSpreadType spread) { spread_ = spread; }

  void addStop(CSVGStop *stop) {
    stops_.push_back(stop);
  }

  bool anyStops() const { return ! stops_.empty(); }

  StopList::const_iterator beginStops() const { return stops_.begin(); }
  StopList::const_iterator endStops  () const { return stops_.end  (); }

  bool processOption(const std::string &name, const std::string &value);

  void termParse();

  bool isDrawable() { return false; }

  void draw();

  void print(std::ostream &os) const;

  CImagePtr getImage(CBBox2D &bbox);

  CRadialGradient *createGradient();

  friend std::ostream &operator<<(std::ostream &os, const CSVGRadialGradient &gradient);
};
