#ifndef CSVG_FILTER_BASE_H
#define CSVG_FILTER_BASE_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGFilterBase : public CSVGObject {
 public:
  CSVGFilterBase(CSVG &svg);
  CSVGFilterBase(const CSVGFilterBase &filter);

  std::string getUniqueName() const;

  CSVGCoordUnits getUnits() const {
    return units_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  CSVGCoordUnits getPrimitiveUnits() const {
    return primitiveUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setPrimitiveUnits(CSVGCoordUnits units) { primitiveUnits_ = units; }

  bool hasX() const { return x_.isValid(); }
  double getX() const { return x_.isValid() ? x_.getValue().value() : 0; }
  void setX(double x) { x_ = x; }

  bool hasY() const { return y_.isValid(); }
  double getY() const { return y_.isValid() ? y_.getValue().value() : 0; }
  void setY(double y) { y_ = y; }

  bool hasWidth() const { return width_.isValid(); }
  double getWidth() const { return width_.getValue(CSVGLengthValue(100)).value(); }
  void setWidth(double w) { width_ = w; }

  bool hasHeight() const { return height_.isValid(); }
  double getHeight() const { return height_.getValue(CSVGLengthValue(100)).value(); }
  void setHeight(double h) { height_ = h; }

  bool hasLink() const { return xlink_.isValid(); }
  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool isFilter() const override { return true; }

  void setObject(CSVGObject *object) { object_ = object; }
  CSVGObject *getObject() const { return object_; }

  bool getParentBBox(CBBox2D &bbox) const;

  CSVGFilter *getParentFilter() const;

  void printValues(std::ostream &os) const;

 protected:
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
