#ifndef CSVG_FILTER_BASE_H
#define CSVG_FILTER_BASE_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGFilterBase : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("filterBase", CSVGObjTypeId::FILTER_BASE)

  CSVGFilterBase(CSVG &svg);
  CSVGFilterBase(const CSVGFilterBase &filter);

  std::string getUniqueName() const override;

  CSVGCoordUnits getFilterUnits() const {
    return filterUnits_.value_or(CSVGCoordUnits::USER_SPACE); }
  void setFilterUnits(const CSVGCoordUnits &units) { filterUnits_ = units; }

  CSVGCoordUnits getPrimitiveUnits() const {
    return primitiveUnits_.value_or(CSVGCoordUnits::USER_SPACE); }
  void setPrimitiveUnits(const CSVGCoordUnits &units) { primitiveUnits_ = units; }

  bool hasX() const { return !!x_; }
  CScreenUnits getX(double w=100) const { return x_.value_or(CScreenUnits(w)); }
  void setX(const CScreenUnits &x) { x_ = x; }

  bool hasY() const { return !!y_; }
  CScreenUnits getY(double h=100) const { return y_.value_or(CScreenUnits(h)); }
  void setY(const CScreenUnits &y) { y_ = y; }

  bool hasWidth() const { return !!width_; }
  CScreenUnits getWidth(double w=100) const { return width_.value_or(CScreenUnits(w)); }
  void setWidth(const CScreenUnits &w) { width_ = w; }

  bool hasHeight() const { return !!height_; }
  CScreenUnits getHeight(double h=100) const { return height_.value_or(CScreenUnits(h)); }
  void setHeight(const CScreenUnits &h) { height_ = h; }

  bool hasLink() const { return !!xlink_; }
  const CSVGXLink &xlink() const { return xlink_.value(); }

  std::string getLinkName() const { return (xlink_ ? xlink_.value().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool isFilter() const override { return true; }

  void setObject(CSVGObject *object) { object_ = object; }
  CSVGObject *getObject() const { return object_; }

  std::string calcFilterIn (const std::optional<std::string> &filterIn ) const;
  std::string calcFilterOut(const std::optional<std::string> &filterOut) const;

  bool getFilterObjectBBox(CBBox2D &bbox) const;

  bool getFilterRegion(CBBox2D &bbox) const;

  bool getBufferSubRegion(CSVGBuffer *inBuffer, CBBox2D &bbox) const;

  virtual bool getSubRegion(CBBox2D &bbox) const;

  bool getBBoxSubRegion(const CBBox2D &inBBox, const CBBox2D &objectBBox, CBBox2D &bbox) const;

  CSVGObject *getParentFilterObject() const;

  CSVGFilter *getParentFilter() const;

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  virtual bool drawElement() = 0;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  CSVGObject*                   object_ { nullptr };
  std::optional<CSVGCoordUnits> filterUnits_;
  std::optional<CSVGCoordUnits> primitiveUnits_;
  std::optional<CScreenUnits>   x_;
  std::optional<CScreenUnits>   y_;
  std::optional<CScreenUnits>   width_;
  std::optional<CScreenUnits>   height_;
  std::optional<std::string>    filterRes_;
  std::optional<CSVGXLink>      xlink_;
};

#endif
