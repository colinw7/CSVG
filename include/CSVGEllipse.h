#ifndef CSVGEllipse_H
#define CSVGEllipse_H

#include <CSVGObject.h>

class CSVGEllipse : public CSVGObject, public CSVGPrintBase<CSVGEllipse> {
 public:
  CSVG_OBJECT_DEF("ellipse", CSVGObjTypeId::ELLIPSE)

  CSVGEllipse(CSVG &svg);
  CSVGEllipse(const CSVGEllipse &ellipse);

  CSVGEllipse *dup() const override;

  //---

  // ellipse (x/y center, x/y radius)

  bool hasCenterX() const { return !!cx_; }
  CScreenUnits getCenterX() const { return cx_.value_or(CScreenUnits(0)); }
  void setCenterX(const CScreenUnits &x, bool update=true) { cx_ = x;
    if (update) updateBBox();
  }

  bool hasCenterY() const { return !!cy_; }
  CScreenUnits getCenterY() const { return cy_.value_or(CScreenUnits(0)); }
  void setCenterY(const CScreenUnits &y, bool update=true) { cy_ = y;
    if (update) updateBBox();
  }

  bool hasRadiusX() const { return !!rx_; }
  CScreenUnits getRadiusX() const { return rx_.value_or(CScreenUnits(1)); }
  void setRadiusX(const CScreenUnits &r) { rx_ = r; updateBBox(); }

  bool hasRadiusY() const { return !!ry_; }
  CScreenUnits getRadiusY() const { return ry_.value_or(CScreenUnits(1)); }
  void setRadiusY(const CScreenUnits &r) { ry_ = r; updateBBox(); }

  CPoint2D getCenter() const;
  void setCenter(const CPoint2D &center);

  void setRadius(double rx, double ry) {
    rx_ = CScreenUnits(rx); ry_ = CScreenUnits(ry);
    updateBBox();
  }

  //---

  bool processOption(const std::string &name, const std::string &value) override;

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
  std::optional<CScreenUnits> cx_;
  std::optional<CScreenUnits> cy_;
  std::optional<CScreenUnits> rx_;
  std::optional<CScreenUnits> ry_;

  mutable CBBox2D          bbox_;
  mutable CSVGPathPartList parts_;
};

#endif
