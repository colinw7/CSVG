#include <CSVGUtil.h>
#include <CMathGen.h>

void
CSVGUtil::
convertArcCoords(double x1, double y1, double x2, double y2, double phi,
                 double rx, double ry, int fa, int fs, bool unit_circle,
                 double *cx, double *cy, double *rx1, double *ry1,
                 double *theta, double *delta)
{
  // start and end are the same so it's a complete ellipse
  if (fabs(x2 - x1) < 1E-6 && fabs(y2 - y1) < 1E-6) {
    if (fs == 1) {
      *cx = x1 + rx;
      *cy = y1;
    }
    else {
      *cx = x1 - rx;
      *cy = y1;
    }

    *rx1 = rx;
    *ry1 = ry;

    *theta = 0;
    *delta = 360;

    return;
  }

  rx = fabs(rx);
  ry = fabs(ry);

  phi = CMathGen::DegToRad(phi);

  double sin_phi = sin(phi);
  double cos_phi = cos(phi);

  double dx = (x1 - x2)/2.0;
  double dy = (y1 - y2)/2.0;

  double dx1 =  cos_phi*dx + sin_phi*dy;
  double dy1 = -sin_phi*dx + cos_phi*dy;

  double rxx = rx*rx;
  double ryy = ry*ry;

  double dxx1 = dx1*dx1;
  double dyy1 = dy1*dy1;

  // Fix radii
  double rcheck = dxx1/rxx + dyy1/ryy;

  if (rcheck > 1) {
    double s = sqrt(rcheck);

    rx *= s;
    ry *= s;

    rxx = rx*rx;
    ryy = ry*ry;
  }

  if (unit_circle) {
    double a00 =  cos_phi/rx;
    double a01 =  sin_phi/rx;
    double a10 = -sin_phi/ry;
    double a11 =  cos_phi/ry;

    /* (xt1, yt1) is current point in transformed coordinate space.
       (xt2, yt2) is new point in transformed coordinate space.

       The arc fits a unit-radius circle in this space.
    */
    double xt1 = a00*x1 + a01*y1;
    double yt1 = a10*x1 + a11*y1;
    double xt2 = a00*x2 + a01*y2;
    double yt2 = a10*x2 + a11*y2;

    double d = (xt2 - xt1)*(xt2 - xt1) + (yt2 - yt1)*(yt2 - yt1);

    double sfactor_sq = 1.0 / d - 0.25;

    if (sfactor_sq < 0) sfactor_sq = 0;

    double sfactor = sqrt(sfactor_sq);

    if (fa == fs) sfactor = -sfactor;

    // (cx, cy) is center of the circle.
    *cx = 0.5*(xt1 + xt2) - sfactor*(yt2 - yt1);
    *cy = 0.5*(yt1 + yt2) + sfactor*(xt2 - xt1);

    // Calculate angles
    *theta = atan2(yt1 - *cy, xt1 - *cx);

    double theta1 = atan2(yt2 - *cy, xt2 - *cx);

    *delta = theta1 - *theta;

    if      (fs == 0 && *delta > 0)
      *delta -= 2*M_PI;
    else if (fs == 1 && *delta < 0)
      *delta += 2*M_PI;

    *theta = CMathGen::RadToDeg(*theta);
    *delta = CMathGen::RadToDeg(*delta);
  }
  else {
    // Calculate center of arc
    double sfactor_sq = (rxx*ryy - rxx*dyy1 - ryy*dxx1)/(rxx*dyy1 + ryy*dxx1);

    if (sfactor_sq < 0) sfactor_sq = 0;

    double sfactor = sqrt(sfactor_sq);

    if (fa == fs) sfactor = -sfactor;

    double cx1 =  sfactor*((rx*dy1)/ry);
    double cy1 = -sfactor*((ry*dx1)/rx);

    double sx2 = (x1 + x2)/2.0;
    double sy2 = (y1 + y2)/2.0;

    *cx = sx2 + cos_phi*cx1 - sin_phi*cy1;
    *cy = sy2 + sin_phi*cx1 + cos_phi*cy1;

    // Calculate arc angles
    double ux = ( dx1 - cx1)/rx;
    double uy = ( dy1 - cy1)/ry;

    double vx = (-dx1 - cx1)/rx;
    double vy = (-dy1 - cy1)/ry;

    double mod_u = sqrt(ux*ux + uy*uy);
    double mod_v = ux;

    int sign = (uy < 0) ? -1 : 1;

    *theta = sign*acos(mod_v/mod_u);
    *theta = CMathGen::RadToDeg(*theta);

    while (*theta >=  360) *theta -= 360;
    while (*theta <= -360) *theta += 360;

    mod_u = sqrt((ux*ux + uy*uy) * (vx*vx + vy*vy));
    mod_v = ux*vx + uy*vy;

    sign = ((ux*vy - uy*vx) < 0) ? -1 : 1;

    *delta = sign*acos(mod_v/mod_u);
    *delta = CMathGen::RadToDeg(*delta);

    if      (fs == 0 && *delta > 0)
      *delta -= 360;
    else if (fs == 1 && *delta < 0)
      *delta += 360;

    while (*delta >=  360) *delta -= 360;
    while (*delta <= -360) *delta += 360;
  }

  *rx1 = rx;
  *ry1 = ry;
}
