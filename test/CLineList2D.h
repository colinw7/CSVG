#ifndef CLINE_LIST_2D_H
#define CLINE_LIST_2D_H

#include <CShape2D.h>
#include <CPoint2D.h>
#include <CLine2D.h>
#include <CTriangle2D.h>
#include <CMathGeom2D.h>
#include <CBBox2D.h>

template<typename T>
class CLineList2DT {
 private:
  typedef CPoint2DT<T> Point;

  typedef std::vector<Point> PointList;

 private:
  PointList points_;

 public:
  CLineList2DT() {
  }

  CLineList2DT(const PointList &points) :
   points_(points) {
  }

  CLineList2DT(const Point *points, uint num_points) :
   points_(&points[0], &points[num_points]) {
  }

  CLineList2DT(const T *x, const T *y, uint num_xy) :
   points_() {
    for (uint i = 0; i < num_xy; ++i)
      points_.push_back(Point(x[i], y[i]));
  }

 ~CLineList2DT() { }

  const PointList &getPoints() const { return points_; }

  uint getNumPoints() const {
    return points_.size();
  }

  const Point &getPoint(uint i) const {
    return points_[i];
  }

  Point getPoint(uint i) {
    return points_[i];
  }

  void getPoint(uint i, T *x, T *y) const {
    *x = points_[i].x;
    *y = points_[i].y;
  }

  void setPoint(uint i, T x, T y) {
    points_[i] = CPoint2D(x, y);
  }

  void setPoint(uint i, const Point &point) {
    points_[i] = point;
  }

  void addPoint(const Point &point) {
    points_.push_back(point);
  }

  CBBox2D getBBox() const {
    CBBox2D bbox;

    typename PointList::const_iterator ps = points_.begin();
    typename PointList::const_iterator pe = points_.end  ();

    for ( ; ps != pe; ++ps)
      bbox.add(*ps);

    return bbox;
  }

  void setBBox(const CBBox2D &bbox) {
    CBBox2D obbox = getBBox();

    T sx = bbox.getWidth () / obbox.getWidth ();
    T sy = bbox.getHeight() / obbox.getHeight();

    typename PointList::iterator ps = points_.begin();
    typename PointList::iterator pe = points_.end  ();

    for ( ; ps != pe; ++ps) {
      Point &p = *ps;

      p.x = sx*(p.x - obbox.getXMin()) + bbox.getXMin();
      p.y = sy*(p.y - obbox.getYMin()) + bbox.getYMin();
    }
  }

  void moveBy(const Point &p) {
    typename PointList::iterator ps = points_.begin();
    typename PointList::iterator pe = points_.end  ();

    for ( ; ps != pe; ++ps)
      *ps += p;
  }

  void resizeBy(const Point &ll, const Point &ur) {
    CBBox2D bbox = getBBox();

    T w = bbox.getWidth ();
    T h = bbox.getHeight();

    T dw = ur.x - ll.x;
    T dh = ur.y - ll.y;

    T sx = (w > 0 ? (w + dw) / w : 1);
    T sy = (h > 0 ? (h + dh) / h : 1);

    typename PointList::iterator ps = points_.begin();
    typename PointList::iterator pe = points_.end  ();

    for ( ; ps != pe; ++ps) {
      Point &p = *ps;

      p.x = sx*(p.x - bbox.getXMin()) + bbox.getXMin() + ll.x;
      p.y = sy*(p.y - bbox.getYMin()) + bbox.getYMin() + ll.y;
    }
  }

  void rotateBy(double da, const Point &o) {
    typename PointList::iterator ps = points_.begin();
    typename PointList::iterator pe = points_.end  ();

    for ( ; ps != pe; ++ps)
      *ps = rotatePoint(*ps, da, o);
  }

  double includedAngle() const {
    uint np = points_.size();

    assert(np >= 3);

    return CMathGeom2D::IncludedAngle(points_[0], points_[1], points_[2]);
  }

  bool arcThrough(T xr, T yr, T *xc, T *yc, T *xt1, T *yt1, T *xt2, T *yt2) {
    uint np = points_.size();

    assert(np >= 3);

    return CMathGeom2D::ArcThrough(points_[0].x, points_[0].y,
                                   points_[1].x, points_[1].y,
                                   points_[2].x, points_[2].y, xr, yr,
                                   xc, yc, xt1, yt1, xt2, yt2);
  }
};

typedef CLineList2DT<double> CLineList2D;

#endif
