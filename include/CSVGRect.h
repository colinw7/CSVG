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

  bool hasX() const { return x_.isValid(); }
  double getX() { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  bool hasY() const { return y_.isValid(); }
  double getY() { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  bool hasWidth() const { return width_.isValid(); }
  CScreenUnits getWidth() const { return width_.getValue(CScreenUnits(0)); }
  void setWidth(double w) { width_ = w; }

  bool hasHeight() const { return height_.isValid(); }
  CScreenUnits getHeight() const { return height_ .getValue(CScreenUnits(0)); }
  void setHeight(double h) { height_ = h; }

  bool hasRX() const { return rx_.isValid(); }
  double getRX() { return rx_.getValue(0); }
  void setRX(double x) { rx_ = x; }

  bool hasRY() const { return ry_.isValid(); }
  double getRY() { return ry_.getValue(0); }
  void setRY(double y) { ry_ = y; }

  void setOrigin(const CPoint2D &point);
  void setSize  (const CSize2D  &size );

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveTo(const CPoint2D &p) override;
  void moveBy(const CVector2D &delta) override;

  void resizeTo(const CSize2D &size) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGRect &rect);

 private:
  void updateBBox();

 private:
  COptReal               x_;
  COptReal               y_;
  COptValT<CScreenUnits> width_;
  COptValT<CScreenUnits> height_;
  COptReal               rx_;
  COptReal               ry_;
  mutable CBBox2D        bbox_;
};

#endif
