#ifndef CSVGUtil_H
#define CSVGUtil_H

#include <cassert>
#include <cmath>

namespace CSVGUtil {
  inline double norm(double x, double low, double high) {
    assert(high != low);

    return (x - low)/(high - low);
  }

  inline double lerp(double value1, double value2, double amt) {
    return value1 + (value2 - value1)*amt;
  }

  inline double map(double value, double low1, double high1, double low2, double high2) {
    return lerp(low2, high2, norm(value, low1, high1));
  }

  void convertArcCoords(double x1, double y1, double x2, double y2, double phi,
                        double rx, double ry, int fa, int fs, bool unit_circle,
                        double *cx, double *cy, double *xr, double *yr,
                        double *theta, double *delta);

  inline int round(double r) { return int(r + 0.5); }

  inline int angleQuadrant(double a) {
    while (a <  0     ) a += 2*M_PI;
    while (a >= 2*M_PI) a -= 2*M_PI;

    if (a <   M_PI/2) return 0;
    if (a <   M_PI  ) return 1;
    if (a < 3*M_PI/2) return 2;

    return 3;
  }

  inline void adjustAngles(double &a1, double &a2) {
    int seg1 = angleQuadrant(a1);
    int seg2 = angleQuadrant(a2);

    if      (seg1 == 0 && seg2 == 3)
      a2 -= 2*M_PI;
    else if (seg1 == 3 && seg2 == 0)
      a2 += 2*M_PI;
  }
}

#endif
