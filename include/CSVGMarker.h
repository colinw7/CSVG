#ifndef CSVGMarker_H
#define CSVGMarker_H

#include <CSVGObject.h>
#include <CSVGOrient.h>

class CSVGMarker : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("marker", CSVGObjTypeId::MARKER)

  CSVGMarker(CSVG &svg);
  CSVGMarker(const CSVGMarker &marker);

  CSVGMarker *dup() const override;

  double getRefX() const { return refX_.getValue(0); }
  void setRefX(double r) { refX_ = r; }

  double getRefY() const { return refY_.getValue(0); }
  void setRefY(double r) { refY_ = r; }

  bool hasMarkerUnits() const { return markerUnits_.isValid(); }
  CSVGCoordUnits getMarkerUnits() const {
    return markerUnits_.getValue(CSVGCoordUnits::STROKE_WIDTH); }
  void setMarkerUnits(CSVGCoordUnits units) { markerUnits_ = units; }

  CScreenUnits getMarkerWidth() const { return markerWidth_.getValue(CScreenUnits(3)); }
  void setMarkerWidth(const CScreenUnits &l) { markerWidth_ = l; }

  CScreenUnits getMarkerHeight() const { return markerHeight_.getValue(CScreenUnits(3)); }
  void setMarkerHeight(const CScreenUnits &l) { markerHeight_ = l; }

  CSVGOrient getOrient() const { return orient_.getValue(CSVGOrient()); }
  void setOrient(const CSVGOrient &orient) { orient_ = orient; }

  CAngle getOrientAngle() const { return getOrient().angle(); }
  void setOrientAngle(const CAngle &a) {
    CSVGOrient orient = getOrient();
    orient.setAngle(a);
    orient_ = orient;
  }

  bool getOrientIsAuto() const { return getOrient().isAuto(); }
  void setOrientIsAuto(bool b) {
    CSVGOrient orient = getOrient();
    orient.setIsAuto(b);
    orient_ = orient;
  }

  CSVGPreserveAspect preserveAspect() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  bool processOption(const std::string &name, const std::string &value) override;

  COptString getNameValue(const std::string &name) const;

  bool canFlatten() const override { return false; }

  bool propagateFlat() const override { return false; }

  bool isDrawable() const override { return false; }

  void drawMarker(double x, double y, double angle);

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMarker &marker);

 private:
  COptReal                     refX_;
  COptReal                     refY_;
  COptValT<CSVGCoordUnits>     markerUnits_;
  COptValT<CScreenUnits>       markerWidth_;
  COptValT<CScreenUnits>       markerHeight_;
  COptValT<CSVGOrient>         orient_;
  COptValT<CSVGPreserveAspect> preserveAspect_;
};

#endif
