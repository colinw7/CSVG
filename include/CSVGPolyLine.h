#ifndef CSVGPolyLine_H
#define CSVGPolyLine_H

#include <CSVGObject.h>

class CSVGPolyLine : public CSVGObject, public CSVGPrintBase<CSVGPolyLine> {
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

  void resizeTo(const CSize2D &size) override;

  void rotateBy(double da) override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  void moveDelta(const CVector2D &delta) override;

 private:
  PointList points_;
};

#endif
