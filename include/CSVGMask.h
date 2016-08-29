#ifndef CSVGMask_H
#define CSVGMask_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CSVGMask : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("mask", CSVGObjTypeId::MASK)

  CSVGMask(CSVG &svg);
  CSVGMask(const CSVGMask &mask);

  CSVGMask *dup() const override;

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  double getWidth() const { return width_.isValid() ? width_ .getValue().px().value() : 1; }
  void setWidth(double w) { width_ = w; }

  double getHeight() const { return height_.isValid() ? height_.getValue().px().value() : 1; }
  void setHeight(double h) { height_ = h; }

  bool getUnitsValid() const { return units_.isValid(); }
  CSVGCoordUnits getUnits() const {
    return units_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getContentUnitsValid() const { return contentUnits_.isValid(); }
  CSVGCoordUnits getContentUnits() const {
    return contentUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setContentUnits(CSVGCoordUnits units) { contentUnits_ = units; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool canFlatten() const override { return false; }

  bool isDrawable() const override { return false; }

  void drawMask(const CSVGObject *object);

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMask &mask);

 private:
  CSVGMask &operator=(const CSVGMask &rhs);

 private:
  CSVGObject *             object_ { 0 };
  COptReal                 x_;
  COptReal                 y_;
  COptValT<CScreenUnits>   width_;
  COptValT<CScreenUnits>   height_;
  COptValT<CSVGCoordUnits> units_;
  COptValT<CSVGCoordUnits> contentUnits_;
};

#endif
