class CSVGPolyLine : public CSVGObject {
 public:
  typedef std::vector<CPoint2D> PointList;

 private:
  PointList points_;

 public:
  CSVG_OBJECT_DEF("polyline", CSVG_OBJ_TYPE_POLYLINE)

  CSVGPolyLine(CSVG &svg);
  CSVGPolyLine(const CSVGPolyLine &poly);

  CSVGPolyLine *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  const PointList &getPoints() const { return points_; }

  void addPoint(const CPoint2D &point);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void moveBy(const CVector2D &delta);
  void resizeTo(const CSize2D &size);

  void rotateBy(double da, const CPoint2D &point);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPolyLine &polyline);
};
