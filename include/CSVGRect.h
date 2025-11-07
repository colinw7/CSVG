#ifndef CSVGRect_H
#define CSVGRect_H

#include <CSVGObject.h>

class CSVGRect : public CSVGObject, public CSVGPrintBase<CSVGRect> {
 public:
  CSVG_OBJECT_DEF("rect", CSVGObjTypeId::RECT)

  CSVGRect(CSVG &svg);
  CSVGRect(const CSVGRect &rect);

  CSVGRect *dup() const override;

  const CBBox2D &getBBox() { return bbox_; }

  //---

  // bbox (x, y, width, height)

  bool hasX() const { return !!x_; }
  CScreenUnits getX() const { return x_.value_or(CScreenUnits(0)); }
  void setX(const CScreenUnits &x) { x_ = x; updateBBox(); }

  bool hasY() const { return !!y_; }
  CScreenUnits getY() const { return y_.value_or(CScreenUnits(0)); }
  void setY(const CScreenUnits &y) { y_ = y; updateBBox(); }

  bool hasWidth() const { return !!width_; }
  CScreenUnits getWidth() const { return width_.value_or(CScreenUnits(0)); }
  void setWidth(const CScreenUnits &w) { width_ = w; updateBBox(); }

  bool hasHeight() const { return !!height_; }
  CScreenUnits getHeight() const { return height_ .value_or(CScreenUnits(0)); }
  void setHeight(const CScreenUnits &h) { height_ = h; updateBBox(); }

  //---

  bool hasRX() const { return !!rx_; }
  double getRX() { return rx_.value_or(0); }
  void setRX(double x) { rx_ = x; } // no bbox change

  bool hasRY() const { return !!ry_; }
  double getRY() { return ry_.value_or(0); }
  void setRY(double y) { ry_ = y; } // no bbox change

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

  void resizeTo(const CSize2D &size) override;

  //---

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  void init();

  void updateBBox();

  void moveDelta(const CVector2D &delta) override;

 private:
  std::optional<CScreenUnits> x_;
  std::optional<CScreenUnits> y_;
  std::optional<CScreenUnits> width_;
  std::optional<CScreenUnits> height_;
  std::optional<double>       rx_;
  std::optional<double>       ry_;

  mutable CBBox2D          bbox_;
  mutable CSVGPathPartList parts_;
};

#endif
