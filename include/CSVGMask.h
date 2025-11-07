#ifndef CSVGMask_H
#define CSVGMask_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CSVGMask : public CSVGObject, public CSVGPrintBase<CSVGMask> {
 public:
  CSVG_OBJECT_DEF("mask", CSVGObjTypeId::MASK)

  CSVGMask(CSVG &svg);
  CSVGMask(const CSVGMask &mask);

  CSVGMask *dup() const override;

  double getX() const { return x_.value_or(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.value_or(0); }
  void setY(double y) { y_ = y; }

  double getWidth() const { return width_ ? width_ .value().px().value() : 1; }
  void setWidth(double w) { width_ = CScreenUnits(w); }

  double getHeight() const { return height_ ? height_.value().px().value() : 1; }
  void setHeight(double h) { height_ = CScreenUnits(h); }

  bool getUnitsValid() const { return !!units_; }
  CSVGCoordUnits getUnits() const {
    return units_.value_or(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getContentUnitsValid() const { return !!contentUnits_; }
  CSVGCoordUnits getContentUnits() const {
    return contentUnits_.value_or(CSVGCoordUnits::USER_SPACE); }
  void setContentUnits(CSVGCoordUnits units) { contentUnits_ = units; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool canFlatten() const override { return false; }

  bool isDrawable() const override { return false; }

  void drawMask(const CSVGObject *object);

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  CSVGMask &operator=(const CSVGMask &rhs);

 private:
  CSVGObject *                  object_ { nullptr };
  std::optional<double>         x_;
  std::optional<double>         y_;
  std::optional<CScreenUnits>   width_;
  std::optional<CScreenUnits>   height_;
  std::optional<CSVGCoordUnits> units_;
  std::optional<CSVGCoordUnits> contentUnits_;
};

#endif
