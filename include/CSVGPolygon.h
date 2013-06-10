class CSVGPolygon : public CSVGObject {
 public:
  typedef std::vector<CPoint2D> PointList;

 private:
  PointList points_;

 public:
  CSVG_OBJECT_DEF("polygon", CSVG_OBJ_TYPE_POLYGON)

  CSVGPolygon(CSVG &svg);
  CSVGPolygon(const CSVGPolygon &poly);

  CSVGPolygon *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  const PointList &getPoints() const { return points_; }

  void addPoint(const CPoint2D &point);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void moveBy(const CVector2D &delta);
  void resizeTo(const CSize2D &size);

  void rotateBy(double da, const CPoint2D &point);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPolygon &polygon);
};
