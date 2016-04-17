#ifndef CSVG_FILTER_H
#define CSVG_FILTER_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGFilter : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("filter", CSVGObjTypeId::FILTER)

  CSVGFilter(CSVG &svg);
  CSVGFilter(const CSVGFilter &filter);

  CSVGFilter *dup() const override;

  CSVGCoordUnits getUnits() const {
    return units_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  CSVGCoordUnits getPrimitiveUnits() const {
    return primitiveUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setPrimitiveUnits(CSVGCoordUnits units) { primitiveUnits_ = units; }

  bool hasX() const { return x_.isValid(); }
  double getX(double x=0) const { return hasX() ? x_.getValue().value() : x; }
  void setX(double x) { x_ = x; }

  bool hasY() const { return y_.isValid(); }
  double getY(double y=0) const { return y_.isValid() ? y_.getValue().value() : y; }
  void setY(double y) { y_ = y; }

  bool hasWidth() const { return width_.isValid(); }
  double getWidth(double w=100) const {
    return width_.getValue(CSVGLengthValue(w)).calcValue(w); }
  void setWidth(double w) { width_ = w; }

  bool hasHeight() const { return height_.isValid(); }
  double getHeight(double h=100) const {
    return height_.getValue(CSVGLengthValue(h)).calcValue(h); }
  void setHeight(double h) { height_ = h; }

  bool processOption(const std::string &name, const std::string &value) override;

  void setObject(CSVGObject *object) { object_ = object; }

  CSVGObject *getObject() const { return object_; }

  void draw() override;

  virtual CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFilter &filter);

 private:
  CSVGFilter &operator=(const CSVGFilter &rhs);

 private:
  CSVGObject*               object_ { 0 };
  COptValT<CSVGCoordUnits>  units_;
  COptValT<CSVGCoordUnits>  primitiveUnits_;
  COptValT<CSVGLengthValue> x_;
  COptValT<CSVGLengthValue> y_;
  COptValT<CSVGLengthValue> width_;
  COptValT<CSVGLengthValue> height_;
  COptValT<std::string>     filterRes_;
  COptValT<CSVGXLink>       xlink_;
};

#endif
