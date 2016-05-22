#ifndef CSVGPattern_H
#define CSVGPattern_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CSVGPattern : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("pattern", CSVGObjTypeId::PATTERN)

  CSVGPattern(CSVG &svg);
  CSVGPattern(const CSVGPattern &pattern);

  CSVGPattern *dup() const override;

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  double getWidth() const { return width_.isValid() ? width_.getValue().px().value() : 1; }
  void setWidth(double w) { width_ = w; }

  double getHeight() const { return height_.isValid() ? height_.getValue().px().value() : 1; }
  void setHeight(double h) { height_ = h; }

  bool getUnitsValid() const { return units_.isValid(); }
  CSVGCoordUnits getUnits() const { return units_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getContentUnitsValid() const { return contentUnits_.isValid(); }
  CSVGCoordUnits getContentUnits() const {
    return contentUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setContentUnits(CSVGCoordUnits units) { contentUnits_ = units; }

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPattern &pattern);

  CSVGObject *getObject();

  void setFillImage  (CSVGObject *parent, CSVGBuffer *buffer, double *w1, double *h1);
  void setStrokeImage(CSVGObject *parent, CSVGBuffer *buffer, double *w1, double *h1);

 private:
  CSVGPattern &operator=(const CSVGPattern &rhs);

 private:
  COptValT<double>         x_;
  COptValT<double>         y_;
  COptValT<CScreenUnits>   width_;
  COptValT<CScreenUnits>   height_;
  COptValT<CSVGCoordUnits> units_;
  COptValT<CSVGCoordUnits> contentUnits_;
  COptValT<CMatrixStack2D> patternTransform_;
  COptValT<CSVGXLink>      xlink_;
};

#endif
