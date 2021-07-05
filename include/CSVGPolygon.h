#ifndef CSVGPolygon_H
#define CSVGPolygon_H

#include <CSVGObject.h>

class CSVGPolygon : public CSVGObject {
 public:
  using PointList = std::vector<CPoint2D>;

 public:
  CSVG_OBJECT_DEF("polygon", CSVGObjTypeId::POLYGON)

  CSVGPolygon(CSVG &svg);
  CSVGPolygon(const CSVGPolygon &poly);

  CSVGPolygon *dup() const override;

  const PointList &getPoints() const { return points_; }
  void setPoints(const PointList &p) { points_ = p; }

  void addPoint(const CPoint2D &point);

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;
  void resizeTo(const CSize2D &size) override;

  void rotateBy(double da, const CPoint2D &point) override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPolygon &polygon);

 private:
  PointList points_;
};

#endif
