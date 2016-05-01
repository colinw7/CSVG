#include <CArcToBezier.h>

CArcToBezier::
CArcToBezier()
{
}

CArcToBezier::
~CArcToBezier()
{
}

void
CArcToBezier::
calc(double x, double y, double xr, double yr, double angle1, double angle2)
{
  x_ = x; y_ = y; xr_ = xr; yr_ = yr; angle1_ = angle1; angle2_ = angle2;

  //------

  angle_diff_ = angle2_ - angle1_;

  while (angle_diff_ > 2.0*M_PI)
    angle_diff_ -= 2.0*M_PI;

  while (angle_diff_ < -2.0*M_PI)
    angle_diff_ += 2.0*M_PI;

  //------

  uint num_beziers = getCalcNumBeziers();

  if (num_beziers > arc_beziers_.size())
    arc_beziers_.resize(num_beziers);

  //------

  double da = angle_diff_/num_beziers;

  double rx1 = 4.0*xr_*tan(da*0.25)/3.0;
  double ry1 = 4.0*yr_*tan(da*0.25)/3.0;

  double s1 = sin(angle1_);
  double c1 = cos(angle1_);

  double x1 = x_ + xr_*c1;
  double y1 = y_ + yr_*s1;

  for (uint i = 1; i <= num_beziers; ++i) {
    double angle = angle1_ + i*da;

    double s2 = sin(angle);
    double c2 = cos(angle);

    /*----*/

    double x4 = x_ + xr_*c2; double y4 = y_ + yr_*s2;
    double x2 = x1 - rx1*s1; double y2 = y1 + ry1*c1;
    double x3 = x4 + rx1*s2; double y3 = y4 - ry1*c2;

    arc_beziers_[i - 1].setPoints(x1, y1, x2, y2, x3, y3, x4, y4);

    /*----*/

    x1 = x4; y1 = y4;
    s1 = s2; c1 = c2;
  }
}

void
CArcToBezier::
calcN(double x, double y, double xr, double yr, double angle1, double angle2)
{
  x_ = x; y_ = y; xr_ = xr; yr_ = yr; angle1_ = angle1; angle2_ = angle2;

  //------

  angle_diff_ = angle1_ - angle2_;

  if      (angle_diff_ > 2.0*M_PI || angle_diff_ < -2.0*M_PI)
    angle_diff_  = 2.0*M_PI;
  else if (angle_diff_ < 0.0)
    angle_diff_ += 2.0*M_PI;

  /*------*/

  uint num_beziers = getCalcNumBeziers();

  if (num_beziers > arc_beziers_.size())
    arc_beziers_.resize(num_beziers);

  /*------*/

  double da = angle_diff_/num_beziers;

  double rx1 = 4.0*xr_*tan(da/4.0)/3.0;
  double ry1 = 4.0*yr_*tan(da/4.0)/3.0;

  double s1 = sin(angle1_);
  double c1 = cos(angle1_);

  double x1 = x_ + xr_*c1;
  double y1 = y_ + xr_*s1;

  for (uint i = 1; i <= num_beziers; ++i) {
    double angle = angle1_ - i*da;

    double s2 = sin(angle);
    double c2 = cos(angle);

    /*----*/

    double x4 = x_ + xr_*c2; double y4 = y_ + yr_*s2;
    double x2 = x1 + rx1*s1; double y2 = y1 - ry1*c1;
    double x3 = x4 - rx1*s2; double y3 = y4 + ry1*c2;

    arc_beziers_[i - 1].setPoints(x1, y1, x2, y2, x3, y3, x4, y4);

    /*----*/

    x1 = x4; y1 = y4;
    s1 = s2; c1 = c2;
  }
}

uint
CArcToBezier::
getCalcNumBeziers()
{
  uint num_beziers = CMathGen::Round(2*angle_diff_/M_PI);

  if (num_beziers <= 0)
    num_beziers = 1;

  //------

  // TODO: Determine optimal number of beziers

  if      (angle_diff_ > 1.5*M_PI)
    num_beziers = 4;
  else if (angle_diff_ > M_PI)
    num_beziers = 3;
  else if (angle_diff_ > 0.5*M_PI)
    num_beziers = 2;
  else
    num_beziers = 1;

  return num_beziers;
}
