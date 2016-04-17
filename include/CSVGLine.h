#ifndef CSVGLine_H
#define CSVGLine_H

#include <CSVGObject.h>

class CSVGLine : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("line", CSVGObjTypeId::LINE)

  CSVGLine(CSVG &svg);
  CSVGLine(const CSVGLine &line);

  CSVGLine *dup() const override;

  double getX1() const { return (x1_.isValid() ? x1_.getValue().value() : 0); }
  double getY1() const { return (y2_.isValid() ? y1_.getValue().value() : 0); }
  double getX2() const { return (x2_.isValid() ? x2_.getValue().value() : 0); }
  double getY2() const { return (y2_.isValid() ? y2_.getValue().value() : 0); }

  void setX1(double x1) { x1_ = x1; }
  void setY1(double y1) { y1_ = y1; }
  void setX2(double x2) { x2_ = x2; }
  void setY2(double y2) { y2_ = y2; }

  CPoint2D getStart() const { return CPoint2D(getX1(), getY1()); }
  CPoint2D getEnd  () const { return CPoint2D(getX2(), getY2()); }

  void setStart(const CPoint2D &start) { setX1(start.x); setY1(start.y); }
  void setEnd  (const CPoint2D &end  ) { setX2(end  .x); setY2(end  .y); }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  bool inside(const CPoint2D &pos) const override;

  void moveBy(const CVector2D &delta) override;
  void resizeTo(const CSize2D &size) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGLine &line);

 private:
  COptValT<CSVGLengthValue> x1_;
  COptValT<CSVGLengthValue> y1_;
  COptValT<CSVGLengthValue> x2_;
  COptValT<CSVGLengthValue> y2_;
};

#endif
