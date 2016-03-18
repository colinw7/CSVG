#ifndef CPATH2D_FLATTENER_H
#define CPATH2D_FLATTENER_H

class CPath2DFlattener {
 public:
  CPath2DFlattener() { }

  virtual ~CPath2DFlattener() { }

  virtual void arcToBeziers(const CPoint2D &c, double xr, double yr, double a1, double a2,
                            std::vector<C3Bezier2D> &beziers);
  virtual void arcNToBeziers(const CPoint2D &c, double xr, double yr, double a1, double a2,
                            std::vector<C3Bezier2D> &beziers);

  virtual void bezierToLines(const C2Bezier2D &bezier, std::vector<CPoint2D> &points,
                             std::vector<double> &gradients);
  virtual void bezierToLines(const C3Bezier2D &bezier, std::vector<CPoint2D> &points,
                             std::vector<double> &gradients);

  virtual void lineToPolygon(const CPoint2D &p1, const CPoint2D &p2,
                             std::vector<CPoint2D> &points);

  virtual void lineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                                 std::vector<CPoint2D> &points);

  virtual double getLineTolerance() const;
};

#endif
