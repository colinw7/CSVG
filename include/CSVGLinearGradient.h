class CSVGStop;
class CLinearGradient;

class CSVGLinearGradient : public CSVGObject {
 public:
  typedef std::vector<CSVGStop *> StopList;

 private:
  COptValT<double>              x1_;
  COptValT<double>              y1_;
  COptValT<double>              x2_;
  COptValT<double>              y2_;
  StopList                      stops_;
  COptValT<CMatrix2D>           gtransform_;
  COptValT<CSVGCoordUnits>      units_;
  COptValT<CGradientSpreadType> spread_;

 public:
  CSVG_OBJECT_DEF("linear_gradient", CSVG_OBJ_TYPE_LINEAR_GRADIENT)

  CSVGLinearGradient(CSVG &svg);
  CSVGLinearGradient(const CSVGLinearGradient &lg);

  CSVGLinearGradient *dup() const;

  bool getBBox(CBBox2D &bbox) const {
    bbox = CBBox2D(getX1(), getY1(), getX2(), getY2());

    return true;
  }

  double getX1() const { return x1_.getValue(); }
  double getY1() const { return y1_.getValue(); }
  double getX2() const { return x2_.getValue(); }
  double getY2() const { return y2_.getValue(); }

  void setX1(double x1) { x1_.setValue(x1); }
  void setY1(double y1) { y1_.setValue(y1); }
  void setX2(double x2) { x2_.setValue(x2); }
  void setY2(double y2) { y2_.setValue(y2); }

  void setBBox(const CBBox2D &bbox) {
    setX1(bbox.getXMin());
    setY1(bbox.getYMin());
    setX2(bbox.getXMax());
    setY2(bbox.getYMax());
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

  CLinearGradient *createGradient();

  friend std::ostream &operator<<(std::ostream &os, const CSVGLinearGradient &gradient);
};
