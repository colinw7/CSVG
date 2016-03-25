#ifndef CSVGEllipse_H
#define CSVGEllipse_H

#include <CSVGObject.h>

class CSVGEllipse : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("ellipse", CSVGObjTypeId::ELLIPSE)

  CSVGEllipse(CSVG &svg);
  CSVGEllipse(const CSVGEllipse &ellipse);

  CSVGEllipse *dup() const override;

  double getCenterX() const { return cx_.getValue(0); }
  void setCenterX(double x) { cx_ = x; }

  double getCenterY() const { return cy_.getValue(0); }
  void setCenterY(double y) { cy_ = y; }

  double getRadiusX() const { return rx_.getValue(1); }
  void setRadiusX(double r) { rx_ = r; }

  double getRadiusY() const { return ry_.getValue(1); }
  void setRadiusY(double r) { ry_ = r; }

  void setCenter(const CPoint2D &center) { setCenterX(center.x); setCenterY(center.y); }
  CPoint2D getCenter() const { return CPoint2D(getCenterX(), getCenterY()); }

  void setRadius(double rx, double ry) { rx_ = rx; ry_ = ry; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;

  void resizeTo(const CSize2D &size) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGEllipse &ellipse);

 private:
  COptValT<double> cx_;
  COptValT<double> cy_;
  COptValT<double> rx_;
  COptValT<double> ry_;
};

#endif
