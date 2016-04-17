#ifndef CSVGRadialGradient_H
#define CSVGRadialGradient_H

#include <CSVGObject.h>
#include <CSVGTypes.h>
#include <CRadialGradient.h>

class CSVGStop;

class CSVGRadialGradient : public CSVGObject {
 public:
  typedef std::vector<CSVGStop *> StopList;

 public:
  CSVG_OBJECT_DEF("radial_gradient", CSVGObjTypeId::RADIAL_GRADIENT)

  CSVGRadialGradient(CSVG &svg);
  CSVGRadialGradient(const CSVGRadialGradient &rg);

  CSVGRadialGradient *dup() const override;

  double getCenterX() const { return cx_.isValid() ? cx_.getValue().value() : 0.5; }
  void setCenterX(double x) { cx_ = x; }

  double getCenterY() const { return cy_.isValid() ? cy_.getValue().value() : 0.5; }
  void setCenterY(double y) { cy_ = y; }

  CPoint2D getCenter() { return CPoint2D(getCenterX(), getCenterY()); }
  void setCenter(const CPoint2D &center) { setCenterX(center.x); setCenterY(center.y); }

  double getRadius() const { return radius_.isValid() ? radius_.getValue().value() : 0.5; }
  void setRadius(double r) { radius_ = r; }

  double getFocusX() const { return focusX_.isValid() ? focusX_.getValue() : getCenterX(); }
  void setFocusX(double x) { focusX_ = x; }

  double getFocusY() const { return focusY_.isValid() ? focusY_.getValue() : getCenterY(); }
  void setFocusY(double y) { focusY_ = y; }

  CPoint2D getFocus() const { return CPoint2D(getFocusX(), getFocusY()); }
  void setFocus(const CPoint2D &focus) { setFocusX(focus.x); setFocusY(focus.y); }

  bool getGTransformValid() const { return gtransform_.isValid(); }
  CMatrix2D getGTransform() const { return gtransform_.getValue(); }
  void setGTransform(const CMatrix2D &gtransform) { gtransform_ = gtransform; }

  bool getUnitsValid() const { return units_.isValid(); }
  CSVGCoordUnits getUnits() const { return units_.getValue(CSVGCoordUnits::OBJECT_BBOX); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getSpreadValid() const { return spread_.isValid(); }
  CGradientSpreadType getSpread() const { return spread_.getValue(); }
  void setSpread(CGradientSpreadType spread) { spread_ = spread; }

  void addStop(CSVGStop *stop) { stops_.push_back(stop); }

  bool anyStops() const { return ! stops_.empty(); }

  const StopList &stops() const { return stops_; }

  //StopList::const_iterator beginStops() const { return stops_.begin(); }
  //StopList::const_iterator endStops  () const { return stops_.end  (); }

  bool processOption(const std::string &name, const std::string &value) override;

  void termParse();

  bool isDrawable() const override { return false; }

  void print(std::ostream &os, bool hier) const override;

  CImagePtr getImage(CBBox2D &bbox);

  CRadialGradient *createGradient(CSVGObject *);

  friend std::ostream &operator<<(std::ostream &os, const CSVGRadialGradient &gradient);

 private:
  COptValT<CSVGLengthValue>     cx_;
  COptValT<CSVGLengthValue>     cy_;
  COptValT<CSVGLengthValue>     radius_;
  COptValT<double>              focusX_;
  COptValT<double>              focusY_;
  StopList                      stops_;
  COptValT<CMatrix2D>           gtransform_;
  COptValT<CSVGCoordUnits>      units_;
  COptValT<CGradientSpreadType> spread_;
};

#endif
