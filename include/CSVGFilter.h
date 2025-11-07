#ifndef CSVG_FILTER_H
#define CSVG_FILTER_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGFilterBase;

class CSVGFilter : public CSVGObject, public CSVGPrintBase<CSVGFilter> {
 public:
  CSVG_OBJECT_DEF("filter", CSVGObjTypeId::FILTER)

  CSVGFilter(CSVG &svg);
  CSVGFilter(const CSVGFilter &filter);

  CSVGFilter *dup() const override;

  CSVGCoordUnits getFilterUnits() const {
    return filterUnits_.value_or(CSVGCoordUnits::OBJECT_BBOX); }
  void setFilterUnits(CSVGCoordUnits units) { filterUnits_ = units; }

  CSVGCoordUnits getPrimitiveUnits() const {
    return primitiveUnits_.value_or(CSVGCoordUnits::USER_SPACE); }
  void setPrimitiveUnits(CSVGCoordUnits units) { primitiveUnits_ = units; }

  bool hasX() const { return !!x_; }
  CScreenUnits getX(const CScreenUnits &x=CScreenUnits(0)) const { return x_.value_or(x); }
  void setX(const CScreenUnits &x) { x_ = x; }

  bool hasY() const { return !!y_; }
  CScreenUnits getY(const CScreenUnits &y=CScreenUnits(0)) const { return y_.value_or(y); }
  void setY(const CScreenUnits &y) { y_ = y; }

  bool hasWidth() const { return !!width_; }
  CScreenUnits getWidth(const CScreenUnits &w=CScreenUnits(0)) const {
    return width_.value_or(w); }
  void setWidth(const CScreenUnits &w) { width_ = w; }

  bool hasHeight() const { return !!height_; }
  CScreenUnits getHeight(const CScreenUnits &h=CScreenUnits(0)) const {
    return height_.value_or(h); }
  void setHeight(const CScreenUnits &h) { height_ = h; }

  const CSVGXLink &xlink() const { return xlink_.value(); }

  std::string getLinkName() const { return (xlink_ ? xlink_.value().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  CSVGObject *getObject() const { return object_; }
  void setObject(CSVGObject *object) { object_ = object; }

  const CBBox2D &getContentsBBox() const { return contentsBBox_; }
  void setContentsBBox(const CBBox2D &bbox) { contentsBBox_ = bbox; }

  const std::string &getLastFilterName() const { return lastFilterName_; }
  void setLastFilterName(const std::string &v) { lastFilterName_ = v; }

  CSVGFilterBase *lastElement() const { return lastElement_; }
  void setLastElement(CSVGFilterBase *p) { lastElement_ = p; }

  const CPoint2D &getOffset() const { return offset_; }
  void setOffset(const CPoint2D &p) { offset_ = p; }

  CBBox2D getObjectBBox() const;
  CBBox2D getObjectBBox(CSVGObject *obj) const;

  bool getRegion(CBBox2D &bbox) const;
  bool getRegion(CSVGObject *obj, CBBox2D &bbox) const;

  bool canFlatten() const override { return false; }

  void initDraw(CSVGBuffer *buffer);
  void termDraw(CSVGBuffer *buffer);

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  CSVGFilter &operator=(const CSVGFilter &rhs);

 private:
  CSVGObject*                   object_ { nullptr };
  std::optional<CSVGCoordUnits> filterUnits_;
  std::optional<CSVGCoordUnits> primitiveUnits_;
  std::optional<CScreenUnits>   x_;
  std::optional<CScreenUnits>   y_;
  std::optional<CScreenUnits>   width_;
  std::optional<CScreenUnits>   height_;
  std::optional<std::string>    filterRes_;
  std::optional<CSVGXLink>      xlink_;
  CBBox2D                       contentsBBox_;
  bool                          oldDrawing_ { false };
  std::string                   lastFilterName_;
  CSVGFilterBase*               lastElement_ { nullptr };
  CPoint2D                      offset_;
};

#endif
