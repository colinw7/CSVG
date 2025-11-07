#ifndef CSVGPattern_H
#define CSVGPattern_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CSVGPattern : public CSVGObject, public CSVGPrintBase<CSVGPattern> {
 public:
  CSVG_OBJECT_DEF("pattern", CSVGObjTypeId::PATTERN)

  CSVGPattern(CSVG &svg);
  CSVGPattern(const CSVGPattern &pattern);

  CSVGPattern *dup() const override;

  bool getXValid() const { return !!x_; }
  CScreenUnits getX() const { return x_.value_or(CScreenUnits(0)); }
  void setX(const CScreenUnits &x) { x_ = x; }

  bool getYValid() const { return !!y_; }
  CScreenUnits getY() const { return y_.value_or(CScreenUnits(0)); }
  void setY(const CScreenUnits &y) { y_ = y; }

  bool getWidthValid() const { return !!width_; }
  CScreenUnits getWidth() const { return width_.value_or(CScreenUnits(0)); }
  void setWidth(const CScreenUnits &w) { width_ = w; }

  bool getHeightValid() const { return !!height_; }
  CScreenUnits getHeight() const { return height_.value_or(CScreenUnits(0)); }
  void setHeight(const CScreenUnits &h) { height_ = h; }

  bool getUnitsValid() const { return !!units_; }
  CSVGCoordUnits getUnits() const { return units_.value_or(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getContentUnitsValid() const { return !!contentUnits_; }
  CSVGCoordUnits getContentUnits() const {
    return contentUnits_.value_or(CSVGCoordUnits::USER_SPACE); }
  void setContentUnits(CSVGCoordUnits units) { contentUnits_ = units; }

  const CSVGXLink &xlink() const { return xlink_.value(); }

  std::string getLinkName() const { return (xlink_ ? xlink_.value().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  bool canFlatten() const override { return false; }

  bool isDrawable() const override { return false; }

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

  CSVGObject *getObject();

  void setFillImage  (CSVGObject *parent, CSVGBuffer *buffer,
                      double *x1, double *y1, double *w1, double *h1);
  void setStrokeImage(CSVGObject *parent, CSVGBuffer *buffer, double *w1, double *h1);

 private:
  CSVGPattern &operator=(const CSVGPattern &rhs);

 private:
  std::optional<CScreenUnits>   x_;
  std::optional<CScreenUnits>   y_;
  std::optional<CScreenUnits>   width_;
  std::optional<CScreenUnits>   height_;
  std::optional<CSVGCoordUnits> units_;
  std::optional<CSVGCoordUnits> contentUnits_;
  std::optional<CMatrixStack2D> patternTransform_;
  std::optional<CSVGXLink>      xlink_;
};

#endif
