#ifndef CILINE_2D_H
#define CILINE_2D_H

#include <CIPoint2D.h>
#include <CIVector2D.h>
#include <CMathGen.h>

template<class T>
class CILine2DT {
 private:
  typedef CIPoint2DT<T>  Point;
  typedef CIVector2DT<T> Vector;
  typedef CILine2DT<T>   Line;

 private:
  Point  p1_, p2_;
  Vector v_;

 public:
  CILine2DT() :
   p1_(), p2_(), v_() {
  }

  CILine2DT(const Line &line) :
    p1_(line.p1_), p2_(line.p2_), v_(line.v_) {
  }

  CILine2DT(T x1, T y1, T x2, T y2) :
   p1_(x1, y1), p2_(x2, y2), v_(x2 - x1, y2 - y1) {
  }

  CILine2DT(const Point &p0, const Point &p1) :
   p1_(p0), p2_(p1), v_(p0, p1) {
  }

  const Point  &start () const { return p1_; }
  const Point  &end   () const { return p2_; }
  const Vector &vector() const { return v_ ; }

  void setStart(const Point &start) { p1_ = start; }
  void setEnd  (const Point &end  ) { p2_ = end  ; }

  void print(std::ostream &os) const {
    os << p1_ << " " << p2_;
  }

  friend std::ostream &operator<<(std::ostream &os, const Line &line) {
    line.print(os);

    return os;
  }
};

typedef CILine2DT<int> CILine2D;

#endif
