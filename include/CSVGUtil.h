#ifndef CSVGUtil_H
#define CSVGUtil_H

namespace CSVGUtil {
  inline double norm(double x, double low, double high) {
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
}

#endif
