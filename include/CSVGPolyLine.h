#ifndef CSVGPolyLine_H
#define CSVGPolyLine_H

#include <CSVGObject.h>

class CSVGPolyLine : public CSVGObject {
 public:
  using PointList = std::vector<CPoint2D>;

 public:
  CSVG_OBJECT_DEF("polyline", CSVGObjTypeId::POLYLINE)

  CSVGPolyLine(CSVG &svg);
  CSVGPolyLine(const CSVGPolyLine &poly);

  CSVGPolyLine *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  const PointList &getPoints() const { return points_; }

  void addPoint(const CPoint2D &point);

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;
  void resizeTo(const CSize2D &size) override;

  void rotateBy(double da, const CPoint2D &point) override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPolyLine &polyline);

 private:
  PointList points_;
};

#endif
