#ifndef CSVGLinearGradient_H
#define CSVGLinearGradient_H

#include <CSVGObject.h>
#include <CSVGTypes.h>
#include <CGenGradient.h>

class CSVGStop;
class CLinearGradient;

class CSVGLinearGradient : public CSVGObject {
 public:
  typedef std::vector<CSVGStop *> StopList;

 public:
  CSVG_OBJECT_DEF("linear_gradient", CSVGObjTypeId::LINEAR_GRADIENT)

  CSVGLinearGradient(CSVG &svg);
  CSVGLinearGradient(const CSVGLinearGradient &lg);

  CSVGLinearGradient *dup() const override;

  bool getBBox(CBBox2D &bbox) const {
    if (! x1_.isValid() || ! y1_.isValid() || ! x2_.isValid() || ! y2_.isValid())
      return false;

    bbox = CBBox2D(getX1(), getY1(), getX2(), getY2());

    return true;
  }

  double getX1() const { return x1_.isValid() ? x1_.getValue().value() : 0; }
  void setX1(double x1) { x1_.setValue(x1); }

  double getY1() const { return y1_.isValid() ? y1_.getValue().value() : 0; }
  void setY1(double y1) { y1_.setValue(y1); }

  double getX2() const { return x2_.isValid() ? x2_.getValue().value() : 1; }
  void setX2(double x2) { x2_.setValue(x2); }

  double getY2() const { return x2_.isValid() ? y2_.getValue().value() : 0; }
  void setY2(double y2) { y2_.setValue(y2); }

  void setBBox(const CBBox2D &bbox) {
    setX1(bbox.getXMin());
    setY1(bbox.getYMin());
    setX2(bbox.getXMax());
    setY2(bbox.getYMax());
  }

  bool getGTransformValid() const { return gtransform_.isValid(); }
  CMatrixStack2D getGTransform() const { return gtransform_.getValue(); }
  void setGTransform(const CMatrixStack2D &gtransform) { gtransform_ = gtransform; }

  bool getUnitsValid() const { return units_.isValid(); }
  CSVGCoordUnits getUnits() const { return units_.getValue(CSVGCoordUnits::OBJECT_BBOX); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getSpreadValid() const { return spread_.isValid(); }
  CGradientSpreadType getSpread() const { return spread_.getValue(CGRADIENT_SPREAD_PAD); }
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

  CLinearGradient *createGradient(CSVGObject *);

  friend std::ostream &operator<<(std::ostream &os, const CSVGLinearGradient &gradient);

 private:
  COptValT<CSVGLengthValue>     x1_;
  COptValT<CSVGLengthValue>     y1_;
  COptValT<CSVGLengthValue>     x2_;
  COptValT<CSVGLengthValue>     y2_;
  StopList                      stops_;
  COptValT<CMatrixStack2D>      gtransform_;
  COptValT<CSVGCoordUnits>      units_;
  COptValT<CGradientSpreadType> spread_;
};

#endif
