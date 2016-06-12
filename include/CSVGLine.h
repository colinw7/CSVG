#ifndef CSVGLine_H
#define CSVGLine_H

#include <CSVGObject.h>

class CSVGLine : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("line", CSVGObjTypeId::LINE)

  CSVGLine(CSVG &svg);
  CSVGLine(const CSVGLine &line);

  CSVGLine *dup() const override;

  CScreenUnits getX1(double x=0) const { return x1_.getValue(CScreenUnits(x)); }
  void setX1(const CScreenUnits &x1) { x1_ = x1; }

  CScreenUnits getY1(double y=0) const { return y1_.getValue(CScreenUnits(y)); }
  void setY1(const CScreenUnits &y1) { y1_ = y1; }

  CScreenUnits getX2(double x=0) const { return x2_.getValue(CScreenUnits(x)); }
  void setX2(const CScreenUnits &x2) { x2_ = x2; }

  CScreenUnits getY2(double y=0) const { return y2_.getValue(CScreenUnits(y)); }
  void setY2(const CScreenUnits &y2) { y2_ = y2; }

  CPoint2D getStart(const CSize2D &size=CSize2D(1, 1)) const {
    return CPoint2D(getX1().pxValue(size.getWidth()), getY1().pxValue(size.getHeight())); }
  CPoint2D getEnd  (const CSize2D &size=CSize2D(1, 1)) const {
    return CPoint2D(getX2().pxValue(size.getWidth()), getY2().pxValue(size.getHeight())); }

  void setStart(const CPoint2D &start) { setX1(start.x); setY1(start.y); }
  void setEnd  (const CPoint2D &end  ) { setX2(end  .x); setY2(end  .y); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  bool inside(const CPoint2D &pos) const override;

  void moveBy(const CVector2D &delta) override;
  void resizeTo(const CSize2D &size) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGLine &line);

 private:
  COptValT<CScreenUnits> x1_;
  COptValT<CScreenUnits> y1_;
  COptValT<CScreenUnits> x2_;
  COptValT<CScreenUnits> y2_;
};

#endif
