#ifndef CSVGRect_H
#define CSVGRect_H

#include <CSVGObject.h>

class CSVGRect : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("rect", CSVGObjTypeId::RECT)

  CSVGRect(CSVG &svg);
  CSVGRect(const CSVGRect &rect);

  CSVGRect *dup() const override;

  const CBBox2D &getBBox() { return bbox_; }

  double getX() { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  double getWidth () { return width_ .getValue(1); }
  void setWidth(double w) { width_ = w; }

  double getHeight() { return height_ .getValue(1); }
  void setHeight(double h) { height_ = h; }

  double getRX() { return rx_.getValue(0); }
  void setRX(double x) { rx_ = x; }

  double getRY() { return ry_.getValue(0); }
  void setRY(double y) { ry_ = y; }

  void setOrigin(const CPoint2D &point);
  void setSize  (const CSize2D  &size );

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;
  void resizeTo(const CSize2D &size) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGRect &rect);

 private:
  void updateBBox();

 private:
  COptValT<double> x_      { 0 };
  COptValT<double> y_      { 0 };
  COptValT<double> width_  { 1 };
  COptValT<double> height_ { 1 };
  COptValT<double> rx_     { 0 };
  COptValT<double> ry_     { 0 };
  mutable CBBox2D  bbox_;
};

#endif
