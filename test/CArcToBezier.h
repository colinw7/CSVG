#ifndef CARC_TO_BEZIER_H
#define CARC_TO_BEZIER_H

#include <C3Bezier2D.h>

class CArcToBezier {
 public:
  typedef std::vector<C3Bezier2D> BezierList;

 protected:
  double     x_, y_, xr_, yr_;
  double     angle1_, angle2_;
  double     angle_diff_;
  BezierList arc_beziers_;

 public:
  CArcToBezier();

  virtual ~CArcToBezier();

  void calc (double x, double y, double xr, double yr, double angle1, double angle2);
  void calcN(double x, double y, double xr, double yr, double angle1, double angle2);

  virtual uint getCalcNumBeziers();

  uint getNumBeziers() const { return arc_beziers_.size(); }

  const C3Bezier2D &getBezier(uint i) const { return arc_beziers_[i]; }
};

#endif
