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

  bool getXValid() const { return x_.isValid(); }
  CScreenUnits getX() const { return x_.getValue(CScreenUnits(0)); }
  void setX(const CScreenUnits &x) { x_ = x; }

  bool getYValid() const { return y_.isValid(); }
  CScreenUnits getY() const { return y_.getValue(CScreenUnits(0)); }
  void setY(const CScreenUnits &y) { y_ = y; }

  bool getWidthValid() const { return width_.isValid(); }
  CScreenUnits getWidth() const { return width_.getValue(CScreenUnits(0)); }
  void setWidth(const CScreenUnits &w) { width_ = w; }

  bool getHeightValid() const { return height_.isValid(); }
  CScreenUnits getHeight() const { return height_.getValue(CScreenUnits(0)); }
  void setHeight(const CScreenUnits &h) { height_ = h; }

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
  COptValT<CScreenUnits>   x_;
  COptValT<CScreenUnits>   y_;
  COptValT<CScreenUnits>   width_;
  COptValT<CScreenUnits>   height_;
  COptValT<CSVGCoordUnits> units_;
  COptValT<CSVGCoordUnits> contentUnits_;
  COptValT<CMatrixStack2D> patternTransform_;
  COptValT<CSVGXLink>      xlink_;
};

#endif
