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

  //---

  // bbox (x, y, width, height)

  bool hasX() const { return x_.isValid(); }
  CScreenUnits getX() const { return x_.getValue(CScreenUnits(0)); }
  void setX(const CScreenUnits &x) { x_ = x; updateBBox(); }

  bool hasY() const { return y_.isValid(); }
  CScreenUnits getY() const { return y_.getValue(CScreenUnits(0)); }
  void setY(const CScreenUnits &y) { y_ = y; updateBBox(); }

  bool hasWidth() const { return width_.isValid(); }
  CScreenUnits getWidth() const { return width_.getValue(CScreenUnits(0)); }
  void setWidth(const CScreenUnits &w) { width_ = w; updateBBox(); }

  bool hasHeight() const { return height_.isValid(); }
  CScreenUnits getHeight() const { return height_ .getValue(CScreenUnits(0)); }
  void setHeight(const CScreenUnits &h) { height_ = h; updateBBox(); }

  //---

  bool hasRX() const { return rx_.isValid(); }
  double getRX() { return rx_.getValue(0); }
  void setRX(double x) { rx_ = x; }

  bool hasRY() const { return ry_.isValid(); }
  double getRY() { return ry_.getValue(0); }
  void setRY(double y) { ry_ = y; }

  //---

  void setOrigin(const CPoint2D &point);
  void setSize  (const CSize2D  &size );

  //---

  bool processOption(const std::string &name, const std::string &value) override;

  void strokeChanged() override;

  //---

  bool draw() override;

  //---

  bool getBBox(CBBox2D &bbox) const override;

  const CSVGPathPartList &getPartList() const override;

  //---

  void moveTo(const CPoint2D &p) override;
  void moveBy(const CVector2D &delta) override;

  void resizeTo(const CSize2D &size) override;

  //---

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGRect &rect);

 private:
  void updateBBox();

 private:
  COptValT<CScreenUnits>   x_;
  COptValT<CScreenUnits>   y_;
  COptValT<CScreenUnits>   width_;
  COptValT<CScreenUnits>   height_;
  COptReal                 rx_;
  COptReal                 ry_;
  mutable CBBox2D          bbox_;
  mutable CSVGPathPartList parts_;
};

#endif
