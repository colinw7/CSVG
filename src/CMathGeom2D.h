#ifndef CMATH_GEOM_2D_H
#define CMATH_GEOM_2D_H

#include <list>
#include <map>
#include <vector>

#include <CPoint2D.h>
#include <CBBox2D.h>
#include <CPolygonOrientation.h>
#include <CLineJoinType.h>

struct CPoint2DParam {
  CPoint2D p;
  double   t;

  CPoint2DParam(const CPoint2D &tp, double tt) :
   p(tp), t(tt) {
  }
};

struct CPoint2DParam2 {
  CPoint2D p;
  double   t1;
  double   t2;

  CPoint2DParam2(const CPoint2D &tp, double tt1, double tt2) :
   p(tp), t1(tt1), t2(tt2) {
  }
};

namespace CMathGeom2D {
  enum ClipZone {
    CLIP_ZONE_LEFT  = (1<<0),
    CLIP_ZONE_RIGHT = (1<<1),
    CLIP_ZONE_TOP   = (1<<2),
    CLIP_ZONE_BOT   = (1<<3),

    CLIP_ZONE_0  = 0,
    CLIP_ZONE_1  = CLIP_ZONE_LEFT,
    CLIP_ZONE_2  = CLIP_ZONE_RIGHT,
    CLIP_ZONE_4  = CLIP_ZONE_TOP,
    CLIP_ZONE_8  = CLIP_ZONE_BOT,
    CLIP_ZONE_5  = (CLIP_ZONE_LEFT  | CLIP_ZONE_TOP),
    CLIP_ZONE_6  = (CLIP_ZONE_RIGHT | CLIP_ZONE_TOP),
    CLIP_ZONE_9  = (CLIP_ZONE_LEFT  | CLIP_ZONE_BOT),
    CLIP_ZONE_10 = (CLIP_ZONE_RIGHT | CLIP_ZONE_BOT)
  };

  enum PointPosition {
    POINT_POSITION_NONE  = -999,
    POINT_POSITION_LEFT  = -1,
    POINT_POSITION_RIGHT = 1,
    POINT_POSITION_ON    = 0
  };

  bool IntersectPolygon(const std::vector<CPoint2D> &points, const CBBox2D &bbox,
                        std::vector<CPoint2D> &ipoints);

  bool IntersectPolygons(const double *x1, const double *y1, uint n1,
                         const double *x2, const double *y2, uint n2,
                         double **xi, double **yi, uint *ni);
  bool IntersectPolygons(const CPoint2D *points1, uint num1, const CPoint2D *points2, uint num2,
                         CPoint2D *ipoints, uint *maxi);
  bool IntersectPolygons(const std::vector<CPoint2D> &points1, const std::vector<CPoint2D> &points2,
                         std::vector<CPoint2D> &ipoints);

  bool CutPolygons(const std::vector<CPoint2D> &points1, const std::vector<CPoint2D> &points2,
                   std::vector<CPoint2D> &opoints);
  bool CutPolygons(const std::vector<CPoint2D> &points1, const std::vector<CPoint2D> &points2,
                   std::vector< std::vector<CPoint2D> > &opoints);

  bool AddPolygons(const std::vector<CPoint2D> &points1, const std::vector<CPoint2D> &points2,
                   std::vector<CPoint2D> &opoints);

  //------

  bool PointInsideConvex(const CPoint2D &point, const std::vector<CPoint2D> &points);
  bool PointInsideConvex(const CPoint2D &point, const CPoint2D *points, uint num_points);
  bool PointInsideConvex(double x, double y, const CPoint2D *points, uint num_points);
  bool PointInsideConvex(double x, double y, const double *xp, const double *yp, uint np);

  bool PointInsideEvenOdd(const CPoint2D &point, const std::vector<CPoint2D> &points);
  bool PointInsideEvenOdd(const CPoint2D &point, const CPoint2D *points, uint num_points);
  bool PointInsideEvenOdd(double x, double y, const CPoint2D *points, uint num_points);
  bool PointInsideEvenOdd(double x, double y, const double *xp, const double *yp, uint np);

  bool PointInsideByAngle(const CPoint2D &point, const std::vector<CPoint2D> &points);
  bool PointInsideByAngle(const CPoint2D &point, const CPoint2D *points, uint num_points);
  bool PointInsideByAngle(double x, double y, const CPoint2D *points, uint num_points);
  bool PointInsideByAngle(double x, double y, const double *xp, const double *yp, uint np);

  bool PointInsideConvex1(double x, double y, const double *xp, const double *yp, uint np);

  bool PointInsideRect(double x, double y, double xmin, double ymin, double xmax, double ymax);
  bool PointInsideRect(const CPoint2D &point, const CBBox2D &rect);

  //------

  bool ArcThrough(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3,
                  double r, CPoint2D &center, CPoint2D &t1, CPoint2D &t2);
  bool ArcThrough(double x1, double y1, double x2, double y2, double x3, double y3, double r,
                  double *xc, double *yc, double *xt1, double *yt1, double *xt2, double *yt2);
  bool ArcThrough(double x1, double y1, double x2, double y2, double x3, double y3,
                  double xr, double yr, double *xc, double *yc, double *xt1,
                  double *yt1, double *xt2, double *yt2);

  bool ConvertFromSVGArc(double x1, double y1, double x2, double y2,
                         double phi, double rx, double ry, int fa, int fs,
                         double *cx, double *cy, double *theta, double *delta);
  bool ConvertToSVGArc(double cx, double cy, double rx, double ry, double theta,
                       double delta, double phi, double *x0, double *y0,
                       double *x1, double *y1, int *fa, int *fs);

  bool LinesAreCoincident(double x11, double y11, double x21, double y21,
                          double x12, double y12, double x22, double y22);
  bool LinesAreCoincident(const CPoint2D &p11, const CPoint2D &p12,
                          const CPoint2D &p21, const CPoint2D &p22);

  bool IntersectLine(double x11, double y11, double x21, double y21, double x12, double y12,
                     double x22, double y22, double *xi, double *yi,
                     double *mu1 = NULL, double *mu2 = NULL);
  bool IntersectLine(const CPoint2D &line1s, const CPoint2D &line1e,
                     const CPoint2D &line2s, const CPoint2D &line2e,
                     CPoint2D *point, double *mu1 = NULL, double *mu2 = NULL);

  CPolygonOrientation PolygonOrientation(const double *x, const double *y, uint num_xy);
  CPolygonOrientation PolygonOrientation(const CPoint2D *points, uint num_points);
  CPolygonOrientation PolygonOrientation(const std::vector<CPoint2D> &points);
  CPolygonOrientation PolygonOrientation(double x1, double y1, double x2, double y2,
                                         double x3, double y3);
  CPolygonOrientation PolygonOrientation(const CPoint2D &point1, const CPoint2D &point2,
                                         const CPoint2D &point3);

  bool PolygonIsConvex(const double *x, const double *y, int num_xy);

  double PolygonArea(double x1, double y1, double x2, double y2, double x3, double y3);
  double PolygonArea(double x1, double y1, double x2, double y2,
                     double x3, double y3, double x4, double y4);
  double PolygonArea(const double *x, const double *y, uint num_xy);
  double PolygonArea(const CPoint2D *points, uint num_points);
  double PolygonArea(const std::vector<CPoint2D> &points);

  double PolygonArea2(double x1, double y1, double x2, double y2,
                      double x3, double y3, double x4, double y4);
  double PolygonArea2(const double *x, const double *y, uint num_xy);
  double PolygonArea2(const CPoint2D *points, uint num_points);
  double PolygonArea2(const std::vector<CPoint2D> &points);
  double PolygonArea2(const std::list<CPoint2D> &points);

  double TriangleArea(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3);
  double TriangleArea(double x1, double y1, double x2, double y2, double x3, double y3);
  double TriangleArea2(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3);
  double TriangleArea2(double x1, double y1, double x2, double y2, double x3, double y3);

  void PolygonCentroid(const double *x, const double *y, int num_xy, double *xc, double *yc);
  void PolygonCentroid(const std::vector<CPoint2D> &points, CPoint2D &p);

  double IncludedAngle(double x1, double y1, double x2, double y2, double x3, double y3);
  double IncludedAngle(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3);
  double IncludedAngle(double x1, double y1, double x2, double y2);

  bool clipLine(int xmin, int ymin, int xmax, int ymax, int *x1, int *y1, int *x2, int *y2);
  bool clipLine(double xmin, double ymin, double xmax, double ymax,
                double *x1, double *y1, double *x2, double *y2);
  bool clipLine1(double xmin, double ymin, double xmax, double ymax,
                 double *x1, double *y1, double *x2, double *y2);

  double PointPointDistance(const CPoint2D &point1, const CPoint2D &point2);

  double Hypot(double a, double b);

  bool PolygonLineIntersect(const double *x, const double *y, uint nxy,
                            double x1, double y1, double x2, double y2,
                            double *xi, double *yi, uint *num_i);

  bool CircleLineIntersect(double xc, double yc, double r,
                           double x1, double y1, double x2, double y2,
                           double *xi1, double *yi1,
                           double *xi2, double *yi2, uint *num_i);

  bool ThreePointCircle(const CPoint2D &point1, const CPoint2D &point2,
                        const CPoint2D &point3, CPoint2D &center, double &radius);
  bool ThreePointCircle(double x1, double y1, double x2, double y2,
                        double x3, double y3, double *xc, double *yc, double *r);

  PointPosition PointLinePosition(const CPoint2D &lpoint1, const CPoint2D &lpoint2,
                                  const CPoint2D &point);

  bool PointLineLeft   (const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point);
  bool PointLineRight  (const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point);
  bool PointLineOn     (const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point);
  bool PointLineLeftOn (const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point);
  bool PointLineRightOn(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point);

  bool Collinear(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3);

  bool IntersectsProperly(const CPoint2D &l1point1, const CPoint2D &l1point2,
                          const CPoint2D &l2point1, const CPoint2D &l2point2);

  bool Intersects(const CPoint2D &l1point1, const CPoint2D &l1point2,
                  const CPoint2D &l2point1, const CPoint2D &l2point2);

  bool PointBetween(const CPoint2D &lpoint1, const CPoint2D &lpoint2, const CPoint2D &point);

  CPoint2D RotatePoint(const CPoint2D &point, double angle, const CPoint2D &o);

  bool clipBySector(double xmin, double ymin, double xmax, double ymax,
                    double *x1, double *y1, double *x2, double *y2,
                    ClipZone *zone1, ClipZone *zone2, bool *intersect);

  ClipZone getClipZone(double x, double y, double xmin, double ymin, double xmax, double ymax);

  bool IsPerpendicular(double x21, double y21, double x32, double y32);

  bool ThreePointCircle1(double x1, double y1, double x2, double y2,
                         double x3, double y3, double *xc, double *yc, double *r);

  bool CircleCircleIntersect(double x1, double y1, double r1, double x2, double y2, double r2,
                             double *xi1, double *yi1, double *xi2, double *yi2);

  void PointsRange(const std::vector<CPoint2D> &points, CPoint2D &min_point,
                   CPoint2D &max_point);
  void PointsRange(const std::vector<CIPoint2D> &points, CIPoint2D &min_point,
                   CIPoint2D &max_point);

  void EllipsePointAtAngle(double cx, double cy, double xr, double yr, double a,
                           double *x, double *y);
}

#include <CLine2D.h>

namespace CMathGeom2D {
  bool PolygonIsConvex(const std::vector<CLine2D> &lines);
  bool PolygonIsConvex(const std::list<CLine2D> &lines);

  bool clipLine(const CLine2D &line, const CBBox2D &bbox, CLine2D &cline);

  bool PointLineDistance(const CPoint2D &point, const CLine2D &line, double *dist);

  bool PolygonLineIntersect(const std::vector<CPoint2D> &points, const CLine2D &line,
                            std::vector<CPoint2D> &ipoints);

  bool PolygonLineIntersect(const std::vector<CPoint2D> &points, const CLine2D &line,
                            std::map<uint,CPoint2D> &ipoints);

  bool SlicePolygonByLines(const std::vector<CPoint2D> &poly, const CLine2D &line,
                           std::vector< std::vector<CPoint2D> > &opolys);

  bool LinesAreCoincident(const CLine2D &line1, const CLine2D &line2);

  bool IntersectLine(const CLine2D &line1, const CLine2D &line2,
                     CPoint2D *point, double *mu1, double *mu2);

  PointPosition PointLinePosition(const CLine2D &line, const CPoint2D &point);

  bool PointLineLeft   (const CLine2D &line, const CPoint2D &point);
  bool PointLineRight  (const CLine2D &line, const CPoint2D &point);
  bool PointLineOn     (const CLine2D &line, const CPoint2D &point);
  bool PointLineLeftOn (const CLine2D &line, const CPoint2D &point);
  bool PointLineRightOn(const CLine2D &line, const CPoint2D &point);
}

#include <C3Bezier2D.h>
#include <C2Bezier2D.h>

namespace CMathGeom2D {
  void ArcToBeziers(double x, double y, double xr, double yr, double angle1, double angle2,
                    std::vector<C3Bezier2D> &beziers);
  void ArcNToBeziers(double x, double y, double xr, double yr, double angle1, double angle2,
                     std::vector<C3Bezier2D> &beziers);

  void BezierToLines(const C3Bezier2D &bezier, std::vector<CPoint2D> &points, double tol=-1.0);
  void BezierToLines(const C2Bezier2D &bezier, std::vector<CPoint2D> &points, double tol=-1.0);

  bool BezierLineIntersect(const C2Bezier2D &bezier, const CLine2D &line,
                           std::vector<CPoint2D> &ipoints, double tol);
  bool BezierLineIntersect(const C2Bezier2D &bezier, const CLine2D &line,
                           std::vector<CPoint2DParam2> &ibpoints, double tol);
  bool BezierLineIntersect(const C2Bezier2D &bezier, const CLine2D &line, double t1, double t2,
                           std::vector<CPoint2DParam2> &ibpoints, double tol);

  bool BezierBezierIntersect(const C2Bezier2D &bezier1, const C2Bezier2D &bezier2,
                             std::vector<CPoint2D> &ipoints, double tol);
  bool BezierBezierIntersect(const C2Bezier2D &bezier1, const C2Bezier2D &bezier2,
                             std::vector<CPoint2DParam2> &ipoints, double tol);
  bool BezierBezierIntersect(const C2Bezier2D &bezier1, double t11, double t12,
                             const C2Bezier2D &bezier2, double t21, double t22,
                             std::vector<CPoint2DParam2> &ipoints, double tol);

  bool BezierLineIntersect(const C3Bezier2D &bezier, const CLine2D &line,
                           std::vector<CPoint2D> &ipoints, double tol);
  bool BezierLineIntersect(const C3Bezier2D &bezier, const CLine2D &line,
                           std::vector<CPoint2DParam2> &ibpoints, double tol);
  bool BezierLineIntersect(const C3Bezier2D &bezier, const CLine2D &line, double t1, double t2,
                           std::vector<CPoint2DParam2> &ibpoints, double tol);

  bool BezierBezierIntersect(const C3Bezier2D &bezier1, const C3Bezier2D &bezier2,
                             std::vector<CPoint2D> &ipoints, double tol);
  bool BezierBezierIntersect(const C3Bezier2D &bezier1, const C3Bezier2D &bezier2,
                             std::vector<CPoint2DParam2> &ipoints, double tol);
  bool BezierBezierIntersect(const C3Bezier2D &bezier1, double t11, double t12,
                             const C3Bezier2D &bezier2, double t21, double t22,
                             std::vector<CPoint2DParam2> &ipoints, double tol);

  void AddUniquePoint(std::vector<CPoint2DParam2> &points, const CPoint2DParam2 &p);

  bool LineToPolygon(const CPoint2D &p1, const CPoint2D &p2,
                     std::vector<CPoint2D> &points, double line_width=1.0);

  bool LineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                         CLineJoinType lineJoin, std::vector<CPoint2D> &points,
                         double line_width=1.0);

  bool MitreLineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                              std::vector<CPoint2D> &points, double line_width=1.0,
                              double mitre_limit=1.0);
  bool RoundLineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                              std::vector<CPoint2D> &points);
  bool BevelLineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                              std::vector<CPoint2D> &points, double line_width=1.0);

  void AddWidthToPoint(const CPoint2D &p, double g, double line_width, CPoint2D &p1, CPoint2D &p2);
}

struct COffsetBezierData {
  std::vector<C3Bezier2D> beziers;
  std::vector<CPoint2D>   points;
};

namespace CMathGeom2D {
  C3Bezier2D offsetBezier(const C3Bezier2D &b, double offset, uint num_samples=32);

  void offsetBezier(const C3Bezier2D &b, double offset, COffsetBezierData &data,
                    uint num_samples=32, int num_curves=1);
}

#endif
