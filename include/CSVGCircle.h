#ifndef CSVGCircle_H
#define CSVGCircle_H

#include <CSVGObject.h>

class CSVGCircle : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("circle", CSVGObjTypeId::CIRCLE)

  CSVGCircle(CSVG &svg);
  CSVGCircle(const CSVGCircle &circle);

  CSVGCircle *dup() const override;

  double getCenterX() const { return cx_.getValue(0); }
  void setCenterX(double x) { cx_ = x; }

  double getCenterY() const { return cy_.getValue(0); }
  void setCenterY(double y) { cy_ = y; }

  bool hasRadius() const { return radius_.isValid(); }
  CScreenUnits getRadius() const { return radius_.getValue(CScreenUnits(1)); }
  void setRadius(const CScreenUnits &r) { radius_ = r; }

  void setCenter(const CPoint2D &center) { setCenterX(center.x); setCenterY(center.y); }
  CPoint2D getCenter() const { return CPoint2D(getCenterX(), getCenterY()); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;

  void resizeTo(const CSize2D &size) override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGCircle &circle);

 private:
  COptReal               cx_;
  COptReal               cy_;
  COptValT<CScreenUnits> radius_;
};

#endif
