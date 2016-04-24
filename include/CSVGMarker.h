#ifndef CSVGMarker_H
#define CSVGMarker_H

#include <CSVGObject.h>

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

  CSVGCoordUnits getUnits() const { return markerUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { markerUnits_ = units; }

  CSVGLengthValue getMarkerWidth() const { return markerWidth_.getValue(CSVGLengthValue(3)); }
  void setMarkerWidth(const CSVGLengthValue &l) { markerWidth_ = l; }

  CSVGLengthValue getMarkerHeight() const { return markerHeight_.getValue(CSVGLengthValue(3)); }
  void setMarkerHeight(const CSVGLengthValue &l) { markerHeight_ = l; }

  std::string getOrient() const { return orient_.getValue(""); }
  void setOrient(const std::string &s) { orient_ = s; }

  CSVGPreserveAspect preserveAspect() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void drawMarker(double x, double y, double angle);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMarker &marker);

 private:
  COptValT<double>             refX_;
  COptValT<double>             refY_;
  COptValT<CSVGCoordUnits>     markerUnits_;
  COptValT<CSVGLengthValue>    markerWidth_;
  COptValT<CSVGLengthValue>    markerHeight_;
  COptValT<std::string>        orient_;
  COptValT<CSVGPreserveAspect> preserveAspect_;
};

#endif
