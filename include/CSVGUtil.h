#ifndef CSVGUtil_H
#define CSVGUtil_H

namespace CSVGUtil {
  void convertArcCoords(double x1, double y1, double x2, double y2, double phi,
                        double rx, double ry, int fa, int fs, bool unit_circle,
                        double *cx, double *cy, double *xr, double *yr,
                        double *theta, double *delta);
}

#endif
