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
  CScreenUnits getX(double w=100) const { return x_.getValue(CScreenUnits(w)); }
  void setX(const CScreenUnits &x) { x_ = x; }

  bool hasY() const { return y_.isValid(); }
  CScreenUnits getY(double h=100) const { return y_.getValue(CScreenUnits(h)); }
  void setY(const CScreenUnits &y) { y_ = y; }

  bool hasWidth() const { return width_.isValid(); }
  CScreenUnits getWidth(double w=100) const { return width_.getValue(CScreenUnits(w)); }
  void setWidth(const CScreenUnits &w) { width_ = w; }

  bool hasHeight() const { return height_.isValid(); }
  CScreenUnits getHeight(double h=100) const { return height_.getValue(CScreenUnits(h)); }
  void setHeight(const CScreenUnits &h) { height_ = h; }

  bool hasLink() const { return xlink_.isValid(); }
  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool isFilter() const override { return true; }

  void setObject(CSVGObject *object) { object_ = object; }
  CSVGObject *getObject() const { return object_; }

  std::string calcFilterIn (const COptString &filterIn ) const;
  std::string calcFilterOut(const COptString &filterOut) const;

  bool getParentBBox(CBBox2D &bbox) const;

  bool getTransformedParentBBox(CBBox2D &bbox) const;

  CSVGObject *getParentFilterObject() const;

  CSVGFilter *getParentFilter() const;

  void printValues(std::ostream &os) const;

 protected:
  CSVGObject*              object_ { 0 };
  COptValT<CSVGCoordUnits> units_;
  COptValT<CSVGCoordUnits> primitiveUnits_;
  COptValT<CScreenUnits>   x_;
  COptValT<CScreenUnits>   y_;
  COptValT<CScreenUnits>   width_;
  COptValT<CScreenUnits>   height_;
  COptString               filterRes_;
  COptValT<CSVGXLink>      xlink_;
};

#endif
