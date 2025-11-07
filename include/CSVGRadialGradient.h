#ifndef CSVGRadialGradient_H
#define CSVGRadialGradient_H

#include <CSVGObject.h>
#include <CSVGTypes.h>
#include <CRadialGradient.h>
#include <optional>

class CSVGStop;

class CSVGRadialGradient : public CSVGObject, public CSVGPrintBase<CSVGRadialGradient> {
 public:
  using StopList = std::vector<CSVGStop *>;

 public:
  CSVG_OBJECT_DEF("radialGradient", CSVGObjTypeId::RADIAL_GRADIENT)

  CSVGRadialGradient(CSVG &svg);
  CSVGRadialGradient(const CSVGRadialGradient &rg);

  CSVGRadialGradient *dup() const override;

  CScreenUnits getCenterX(double x=0.5) const { return cx_.value_or(CScreenUnits(x)); }
  void setCenterX(const CScreenUnits &x) { cx_ = x; }

  CScreenUnits getCenterY(double y=0.5) const { return cy_.value_or(CScreenUnits(y)); }
  void setCenterY(const CScreenUnits &y) { cy_ = y; }

  CPoint2D getCenter() {
    return CPoint2D(getCenterX().pxValue(CScreenUnits(1)),
                    getCenterY().pxValue(CScreenUnits(1)));
  }
  void setCenter(const CPoint2D &center) {
    setCenterX(CScreenUnits(center.x)); setCenterY(CScreenUnits(center.y)); }

  CScreenUnits getRadius(double r=0.5) const { return radius_.value_or(CScreenUnits(r)); }
  void setRadius(const CScreenUnits &r) { radius_ = r; }

  double getFocusX() const {
    return focusX_ ? focusX_.value() : getCenterX().pxValue(CScreenUnits(1));
  }
  void setFocusX(double x) { focusX_ = x; }

  double getFocusY() const {
    return focusY_? focusY_.value() : getCenterY().pxValue(CScreenUnits(1));
  }
  void setFocusY(double y) { focusY_ = y; }

  CPoint2D getFocus() const { return CPoint2D(getFocusX(), getFocusY()); }
  void setFocus(const CPoint2D &focus) { setFocusX(focus.x); setFocusY(focus.y); }

  bool getGTransformValid() const { return !!gtransform_; }
  CMatrixStack2D getGTransform() const { return gtransform_.value(); }
  void setGTransform(const CMatrixStack2D &gtransform) { gtransform_ = gtransform; }

  bool getUnitsValid() const { return !!units_; }
  CSVGCoordUnits getUnits() const { return units_.value_or(CSVGCoordUnits::OBJECT_BBOX); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getSpreadValid() const { return !!spread_; }
  CGradientSpreadType getSpread() const { return spread_.value(); }
  void setSpread(CGradientSpreadType spread) { spread_ = spread; }

  void addStop(CSVGStop *stop) { stops_.push_back(stop); }

  bool anyStops() const { return ! stops_.empty(); }

  const StopList &stops() const { return stops_; }

  //StopList::const_iterator beginStops() const { return stops_.begin(); }
  //StopList::const_iterator endStops  () const { return stops_.end  (); }

  bool processOption(const std::string &name, const std::string &value) override;

  void termParse() override;

  bool canFlatten() const override { return false; }

  bool isDrawable() const override { return false; }

  void addLinkStops();

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

  void setFillBuffer  (CSVGBuffer *buffer, CSVGObject *obj, const std::optional<double> &opacity);
  void setStrokeBuffer(CSVGBuffer *buffer, CSVGObject *obj, const std::optional<double> &opacity);

  CRadialGradient *createGradient(CSVGObject *obj, const std::optional<double> &opacity);

  void getControlPoints(CSVGObject *obj, double *xc, double *yc, double *r,
                        double *xf, double *yf);

 private:
  std::optional<CScreenUnits>        cx_;
  std::optional<CScreenUnits>        cy_;
  std::optional<CScreenUnits>        radius_;
  std::optional<double>              focusX_;
  std::optional<double>              focusY_;
  StopList                           stops_;
  std::optional<CMatrixStack2D>      gtransform_;
  std::optional<CSVGCoordUnits>      units_;
  std::optional<CGradientSpreadType> spread_;
  std::optional<CSVGXLink>           xlink_;
};

#endif
