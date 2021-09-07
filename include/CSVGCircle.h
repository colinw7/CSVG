#ifndef CSVGCircle_H
#define CSVGCircle_H

#include <CSVGObject.h>

class CSVGCircle : public CSVGObject, public CSVGPrintBase<CSVGCircle> {
 public:
  CSVG_OBJECT_DEF("circle", CSVGObjTypeId::CIRCLE)

  CSVGCircle(CSVG &svg);
  CSVGCircle(const CSVGCircle &circle);

  CSVGCircle *dup() const override;

  //---

  // circle (x/y center, radius)

  bool hasCenterX() const { return cx_.isValid(); }
  CScreenUnits getCenterX() const { return cx_.getValue(CScreenUnits(0)); }
  void setCenterX(const CScreenUnits &x, bool update=true) { cx_ = x;
    if (update) updateBBox();
  }

  bool hasCenterY() const { return cy_.isValid(); }
  CScreenUnits getCenterY() const { return cy_.getValue(CScreenUnits(0)); }
  void setCenterY(const CScreenUnits &y, bool update=true) { cy_ = y;
    if (update) updateBBox();
  }

  bool hasRadius() const { return radius_.isValid(); }
  CScreenUnits getRadius() const { return radius_.getValue(CScreenUnits(1)); }
  void setRadius(const CScreenUnits &r) { radius_ = r; updateBBox(); }

  CPoint2D getCenter() const;
  void setCenter(const CPoint2D &center);

  //---

  bool processOption(const std::string &name, const std::string &value) override;

  COptString getNameValue(const std::string &name) const override;

  //---

  bool draw() override;

  //---

  bool getBBox(CBBox2D &bbox) const override;

  const CSVGPathPartList &getPartList() const override;

  //---

  void moveBy(const CVector2D &delta) override;

  void resizeTo(const CSize2D &size) override;

  //---

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  void init();

  void updateBBox();

 private:
  COptScreenUnits cx_;
  COptScreenUnits cy_;
  COptScreenUnits radius_;

  mutable CBBox2D          bbox_;
  mutable CSVGPathPartList parts_;
};

#endif
