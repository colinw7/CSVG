#ifndef CSVGCircle_H
#define CSVGCircle_H

#include <CSVGObject.h>

class CSVGCircle : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("circle", CSVGObjTypeId::CIRCLE)

  CSVGCircle(CSVG &svg);
  CSVGCircle(const CSVGCircle &circle);

  CSVGCircle *dup() const override;

  bool hasCenterX() const { return cx_.isValid(); }
  CScreenUnits getCenterX() const { return cx_.getValue(CScreenUnits(0)); }
  void setCenterX(const CScreenUnits &x) { cx_ = x; }

  bool hasCenterY() const { return cy_.isValid(); }
  CScreenUnits getCenterY() const { return cy_.getValue(CScreenUnits(0)); }
  void setCenterY(const CScreenUnits &y) { cy_ = y; }

  bool hasRadius() const { return radius_.isValid(); }
  CScreenUnits getRadius() const { return radius_.getValue(CScreenUnits(1)); }
  void setRadius(const CScreenUnits &r) { radius_ = r; }

  CPoint2D getCenter() const;
  void setCenter(const CPoint2D &center);

  bool processOption(const std::string &name, const std::string &value) override;

  COptString getNameValue(const std::string &name) const override;

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;

  void resizeTo(const CSize2D &size) override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGCircle &circle);

 private:
  COptScreenUnits cx_;
  COptScreenUnits cy_;
  COptScreenUnits radius_;
};

#endif
