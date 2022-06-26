#ifndef CQSVGPathPart_H
#define CQSVGPathPart_H

#include <CSVGPathPart.h>
#include <QObject>

class CQSVG;

class CQSVGPathPart : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString type READ getTypeName)

 public:
  CQSVGPathPart();

  virtual ~CQSVGPathPart() { }

  virtual CSVGPathPartType getType() const = 0;

  QString getTypeName() const { return CSVGPathPart::partTypeName(getType()).c_str(); }
};

//---

class CQSVGPathMoveTo : public CQSVGPathPart, public CSVGPathMoveTo {
  Q_OBJECT

  Q_PROPERTY(double x READ getX WRITE setX)
  Q_PROPERTY(double y READ getY WRITE setY)

 public:
  CQSVGPathMoveTo(CQSVG &svg, double x, double y);

  CSVGPathPartType getType() const { return CSVGPathMoveTo::getType(); }

  double getX() const { return CSVGPathMoveTo::getPoint().x; }
  double getY() const { return CSVGPathMoveTo::getPoint().y; }

  void setX(double x) { CSVGPathMoveTo::setPoint(CPoint2D(x, getY())); }
  void setY(double y) { CSVGPathMoveTo::setPoint(CPoint2D(getX(), y)); }
};

//---

class CQSVGPathRMoveTo : public CQSVGPathPart, public CSVGPathRMoveTo {
  Q_OBJECT

  Q_PROPERTY(double dx READ getDX WRITE setDX)
  Q_PROPERTY(double dy READ getDY WRITE setDY)

 public:
  CQSVGPathRMoveTo(CQSVG &svg, double x, double y);

  CSVGPathPartType getType() const { return CSVGPathRMoveTo::getType(); }

  double getDX() const { return CSVGPathRMoveTo::getPoint().x; }
  double getDY() const { return CSVGPathRMoveTo::getPoint().y; }

  void setDX(double x) { CSVGPathRMoveTo::setPoint(CPoint2D(x, getDY())); }
  void setDY(double y) { CSVGPathRMoveTo::setPoint(CPoint2D(getDX(), y)); }
};

//---

class CQSVGPathLineTo : public CQSVGPathPart, public CSVGPathLineTo {
  Q_OBJECT

  Q_PROPERTY(double x READ getX WRITE setX)
  Q_PROPERTY(double y READ getY WRITE setY)

 public:
  CQSVGPathLineTo(CQSVG &svg, double x, double y);

  CSVGPathPartType getType() const { return CSVGPathLineTo::getType(); }

  double getX() const { return CSVGPathLineTo::getPoint().x; }
  double getY() const { return CSVGPathLineTo::getPoint().y; }

  void setX(double x) { CSVGPathLineTo::setPoint(CPoint2D(x, getY())); }
  void setY(double y) { CSVGPathLineTo::setPoint(CPoint2D(getX(), y)); }
};

//---

class CQSVGPathRLineTo : public CQSVGPathPart, public CSVGPathRLineTo {
  Q_OBJECT

  Q_PROPERTY(double dx READ getDX WRITE setDX)
  Q_PROPERTY(double dy READ getDY WRITE setDY)

 public:
  CQSVGPathRLineTo(CQSVG &svg, double x, double y);

  CSVGPathPartType getType() const { return CSVGPathRLineTo::getType(); }

  double getDX() const { return CSVGPathRLineTo::getPoint().x; }
  double getDY() const { return CSVGPathRLineTo::getPoint().y; }

  void setDX(double x) { CSVGPathRLineTo::setPoint(CPoint2D(x, getDY())); }
  void setDY(double y) { CSVGPathRLineTo::setPoint(CPoint2D(getDX(), y)); }
};

//---

class CQSVGPathHLineTo : public CQSVGPathPart, public CSVGPathHLineTo {
  Q_OBJECT

  Q_PROPERTY(double x READ getX WRITE setX)

 public:
  CQSVGPathHLineTo(CQSVG &svg, double x);

  CSVGPathPartType getType() const { return CSVGPathHLineTo::getType(); }

  double getX() const { return CSVGPathHLineTo::getX(); }
  void setX(double x) { CSVGPathHLineTo::setX(x); }
};

//---

class CQSVGPathRHLineTo : public CQSVGPathPart, public CSVGPathRHLineTo {
  Q_OBJECT

  Q_PROPERTY(double dx READ getDX WRITE setDX)

 public:
  CQSVGPathRHLineTo(CQSVG &svg, double dx);

  CSVGPathPartType getType() const { return CSVGPathRHLineTo::getType(); }

  double getDX() const { return CSVGPathRHLineTo::getDX(); }
  void setDX(double x) { CSVGPathRHLineTo::setDX(x); }
};

//---

class CQSVGPathVLineTo : public CQSVGPathPart, public CSVGPathVLineTo {
  Q_OBJECT

  Q_PROPERTY(double y READ getY WRITE setY)

 public:
  CQSVGPathVLineTo(CQSVG &svg, double y);

  CSVGPathPartType getType() const { return CSVGPathVLineTo::getType(); }

  double getY() const { return CSVGPathVLineTo::getY(); }
  void setY(double y) { CSVGPathVLineTo::setY(y); }
};

//---

class CQSVGPathRVLineTo : public CQSVGPathPart, public CSVGPathRVLineTo {
  Q_OBJECT

  Q_PROPERTY(double dy READ getDY WRITE setDY)

 public:
  CQSVGPathRVLineTo(CQSVG &svg, double dy);

  CSVGPathPartType getType() const { return CSVGPathRVLineTo::getType(); }

  double getDY() const { return CSVGPathRVLineTo::getDY(); }
  void setDY(double y) { CSVGPathRVLineTo::setDY(y); }
};

//---

class CQSVGPathArcTo : public CQSVGPathPart, public CSVGPathArcTo {
  Q_OBJECT

  Q_PROPERTY(double rx READ getRX WRITE setRX)
  Q_PROPERTY(double ry READ getRY WRITE setRY)
  Q_PROPERTY(double xa READ getXA WRITE setXA)
  Q_PROPERTY(int    fa READ getFA WRITE setFA)
  Q_PROPERTY(int    fs READ getFS WRITE setFS)
  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)

 public:
  CQSVGPathArcTo(CQSVG &svg, double rx, double ry, double xa,
                 int fa, int fs, double x2, double y2);

  CSVGPathPartType getType() const { return CSVGPathArcTo::getType(); }

  double getRX() const { return CSVGPathArcTo::getRadiusX(); }
  void setRX(double r) { CSVGPathArcTo::setRadiusX(r); }

  double getRY() const { return CSVGPathArcTo::getRadiusY(); }
  void setRY(double r) { CSVGPathArcTo::setRadiusY(r); }

  double getXA() const { return CSVGPathArcTo::getXA(); }
  void setXA(double r) { CSVGPathArcTo::setXA(r); }

  int getFA() const { return CSVGPathArcTo::getFA(); }
  void setFA(int i) { CSVGPathArcTo::setFA(i); }

  int getFS() const { return CSVGPathArcTo::getFS(); }
  void setFS(int i) { CSVGPathArcTo::setFS(i); }

  double getX2() const { return CSVGPathArcTo::getPoint2().x; }
  void setX2(double x) { CSVGPathArcTo::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathArcTo::getPoint2().y; }
  void setY2(double y) { CSVGPathArcTo::setPoint2(CPoint2D(getX2(), y)); }
};

//---

class CQSVGPathRArcTo : public CQSVGPathPart, public CSVGPathRArcTo {
  Q_OBJECT

  Q_PROPERTY(double rx READ getRX WRITE setRX)
  Q_PROPERTY(double ry READ getRY WRITE setRY)
  Q_PROPERTY(double xa READ getXA WRITE setXA)
  Q_PROPERTY(int    fa READ getFA WRITE setFA)
  Q_PROPERTY(int    fs READ getFS WRITE setFS)
  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)

 public:
  CQSVGPathRArcTo(CQSVG &svg, double rx, double ry, double xa,
                  int fa, int fs, double x2, double y2);

  CSVGPathPartType getType() const { return CSVGPathRArcTo::getType(); }

  double getRX() const { return CSVGPathRArcTo::getRadiusX(); }
  void setRX(double r) { CSVGPathRArcTo::setRadiusX(r); }

  double getRY() const { return CSVGPathRArcTo::getRadiusY(); }
  void setRY(double r) { CSVGPathRArcTo::setRadiusY(r); }

  double getXA() const { return CSVGPathRArcTo::getXA(); }
  void setXA(double r) { CSVGPathRArcTo::setXA(r); }

  int getFA() const { return CSVGPathRArcTo::getFA(); }
  void setFA(int i) { CSVGPathRArcTo::setFA(i); }

  int getFS() const { return CSVGPathRArcTo::getFS(); }
  void setFS(int i) { CSVGPathRArcTo::setFS(i); }

  double getX2() const { return CSVGPathRArcTo::getPoint2().x; }
  void setX2(double x) { CSVGPathRArcTo::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathRArcTo::getPoint2().y; }
  void setY2(double y) { CSVGPathRArcTo::setPoint2(CPoint2D(getX2(), y)); }
};

//---

class CQSVGPathBezier2To : public CQSVGPathPart, public CSVGPathBezier2To {
  Q_OBJECT

  Q_PROPERTY(double x1 READ getX1 WRITE setX1)
  Q_PROPERTY(double y1 READ getY1 WRITE setY1)
  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)

 public:
  CQSVGPathBezier2To(CQSVG &svg, double x1, double y1, double x2, double y2);

  CSVGPathPartType getType() const { return CSVGPathBezier2To::getType(); }

  double getX1() const { return CSVGPathBezier2To::getPoint1().x; }
  void setX1(double x) { CSVGPathBezier2To::setPoint1(CPoint2D(x, getY1())); }

  double getY1() const { return CSVGPathBezier2To::getPoint1().y; }
  void setY1(double y) { CSVGPathBezier2To::setPoint1(CPoint2D(getX1(), y)); }

  double getX2() const { return CSVGPathBezier2To::getPoint2().x; }
  void setX2(double x) { CSVGPathBezier2To::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathBezier2To::getPoint2().y; }
  void setY2(double y) { CSVGPathBezier2To::setPoint2(CPoint2D(getX2(), y)); }
};

//---

class CQSVGPathMBezier2To : public CQSVGPathPart, public CSVGPathMBezier2To {
  Q_OBJECT

  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)

 public:
  CQSVGPathMBezier2To(CQSVG &svg, double x2, double y2);

  CSVGPathPartType getType() const { return CSVGPathMBezier2To::getType(); }

  double getX2() const { return CSVGPathMBezier2To::getPoint2().x; }
  void setX2(double x) { CSVGPathMBezier2To::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathMBezier2To::getPoint2().y; }
  void setY2(double y) { CSVGPathMBezier2To::setPoint2(CPoint2D(getX2(), y)); }
};

//---

class CQSVGPathRBezier2To : public CQSVGPathPart, public CSVGPathRBezier2To {
  Q_OBJECT

  Q_PROPERTY(double x1 READ getX1 WRITE setX1)
  Q_PROPERTY(double y1 READ getY1 WRITE setY1)
  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)

 public:
  CQSVGPathRBezier2To(CQSVG &svg, double x1, double y1, double x2, double y2);

  CSVGPathPartType getType() const { return CSVGPathRBezier2To::getType(); }

  double getX1() const { return CSVGPathRBezier2To::getPoint1().x; }
  void setX1(double x) { CSVGPathRBezier2To::setPoint1(CPoint2D(x, getY1())); }

  double getY1() const { return CSVGPathRBezier2To::getPoint1().y; }
  void setY1(double y) { CSVGPathRBezier2To::setPoint1(CPoint2D(getX1(), y)); }

  double getX2() const { return CSVGPathRBezier2To::getPoint2().x; }
  void setX2(double x) { CSVGPathRBezier2To::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathRBezier2To::getPoint2().y; }
  void setY2(double y) { CSVGPathRBezier2To::setPoint2(CPoint2D(getX2(), y)); }
};

//---

class CQSVGPathMRBezier2To : public CQSVGPathPart, public CSVGPathMRBezier2To {
  Q_OBJECT

  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)

 public:
  CQSVGPathMRBezier2To(CQSVG &svg, double x2, double y2);

  CSVGPathPartType getType() const { return CSVGPathMRBezier2To::getType(); }

  double getX2() const { return CSVGPathMRBezier2To::getPoint2().x; }
  void setX2(double x) { CSVGPathMRBezier2To::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathMRBezier2To::getPoint2().y; }
  void setY2(double y) { CSVGPathMRBezier2To::setPoint2(CPoint2D(getX2(), y)); }
};

//---

class CQSVGPathBezier3To : public CQSVGPathPart, public CSVGPathBezier3To {
  Q_OBJECT

  Q_PROPERTY(double x1 READ getX1 WRITE setX1)
  Q_PROPERTY(double y1 READ getY1 WRITE setY1)
  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)
  Q_PROPERTY(double x3 READ getX3 WRITE setX3)
  Q_PROPERTY(double y3 READ getY3 WRITE setY3)

 public:
  CQSVGPathBezier3To(CQSVG &svg, double x1, double y1, double x2, double y2,
                     double x3, double y3);

  CSVGPathPartType getType() const { return CSVGPathBezier3To::getType(); }

  double getX1() const { return CSVGPathBezier3To::getPoint1().x; }
  void setX1(double x) { CSVGPathBezier3To::setPoint1(CPoint2D(x, getY1())); }

  double getY1() const { return CSVGPathBezier3To::getPoint1().y; }
  void setY1(double y) { CSVGPathBezier3To::setPoint1(CPoint2D(getX1(), y)); }

  double getX2() const { return CSVGPathBezier3To::getPoint2().x; }
  void setX2(double x) { CSVGPathBezier3To::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathBezier3To::getPoint2().y; }
  void setY2(double y) { CSVGPathBezier3To::setPoint2(CPoint2D(getX2(), y)); }

  double getX3() const { return CSVGPathBezier3To::getPoint3().x; }
  void setX3(double x) { CSVGPathBezier3To::setPoint3(CPoint2D(x, getY3())); }

  double getY3() const { return CSVGPathBezier3To::getPoint3().y; }
  void setY3(double y) { CSVGPathBezier3To::setPoint3(CPoint2D(getX3(), y)); }
};

//---

class CQSVGPathMBezier3To : public CQSVGPathPart, public CSVGPathMBezier3To {
  Q_OBJECT

  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)
  Q_PROPERTY(double x3 READ getX3 WRITE setX3)
  Q_PROPERTY(double y3 READ getY3 WRITE setY3)

 public:
  CQSVGPathMBezier3To(CQSVG &svg, double x2, double y2, double x3, double y3);

  CSVGPathPartType getType() const { return CSVGPathMBezier3To::getType(); }

  double getX2() const { return CSVGPathMBezier3To::getPoint2().x; }
  void setX2(double x) { CSVGPathMBezier3To::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathMBezier3To::getPoint2().y; }
  void setY2(double y) { CSVGPathMBezier3To::setPoint2(CPoint2D(getX2(), y)); }

  double getX3() const { return CSVGPathMBezier3To::getPoint3().x; }
  void setX3(double x) { CSVGPathMBezier3To::setPoint3(CPoint2D(x, getY3())); }

  double getY3() const { return CSVGPathMBezier3To::getPoint3().y; }
  void setY3(double y) { CSVGPathMBezier3To::setPoint3(CPoint2D(getX3(), y)); }
};

//---

class CQSVGPathRBezier3To : public CQSVGPathPart, public CSVGPathRBezier3To {
  Q_OBJECT

  Q_PROPERTY(double x1 READ getX1 WRITE setX1)
  Q_PROPERTY(double y1 READ getY1 WRITE setY1)
  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)
  Q_PROPERTY(double x3 READ getX3 WRITE setX3)
  Q_PROPERTY(double y3 READ getY3 WRITE setY3)

 public:
  CQSVGPathRBezier3To(CQSVG &svg, double x1, double y1, double x2, double y2,
                      double x3, double y3);

  CSVGPathPartType getType() const { return CSVGPathRBezier3To::getType(); }

  double getX1() const { return CSVGPathRBezier3To::getPoint1().x; }
  void setX1(double x) { CSVGPathRBezier3To::setPoint1(CPoint2D(x, getY1())); }

  double getY1() const { return CSVGPathRBezier3To::getPoint1().y; }
  void setY1(double y) { CSVGPathRBezier3To::setPoint1(CPoint2D(getX1(), y)); }

  double getX2() const { return CSVGPathRBezier3To::getPoint2().x; }
  void setX2(double x) { CSVGPathRBezier3To::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathRBezier3To::getPoint2().y; }
  void setY2(double y) { CSVGPathRBezier3To::setPoint2(CPoint2D(getX2(), y)); }

  double getX3() const { return CSVGPathRBezier3To::getPoint3().x; }
  void setX3(double x) { CSVGPathRBezier3To::setPoint3(CPoint2D(x, getY3())); }

  double getY3() const { return CSVGPathRBezier3To::getPoint3().y; }
  void setY3(double y) { CSVGPathRBezier3To::setPoint3(CPoint2D(getX3(), y)); }
};

//---

class CQSVGPathMRBezier3To : public CQSVGPathPart, public CSVGPathMRBezier3To {
  Q_OBJECT

  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)
  Q_PROPERTY(double x3 READ getX3 WRITE setX3)
  Q_PROPERTY(double y3 READ getY3 WRITE setY3)

 public:
  CQSVGPathMRBezier3To(CQSVG &svg, double x2, double y2, double x3, double y3);

  CSVGPathPartType getType() const { return CSVGPathMRBezier3To::getType(); }

  double getX2() const { return CSVGPathMRBezier3To::getPoint2().x; }
  void setX2(double x) { CSVGPathMRBezier3To::setPoint2(CPoint2D(x, getY2())); }

  double getY2() const { return CSVGPathMRBezier3To::getPoint2().y; }
  void setY2(double y) { CSVGPathMRBezier3To::setPoint2(CPoint2D(getX2(), y)); }

  double getX3() const { return CSVGPathMRBezier3To::getPoint3().x; }
  void setX3(double x) { CSVGPathMRBezier3To::setPoint3(CPoint2D(x, getY3())); }

  double getY3() const { return CSVGPathMRBezier3To::getPoint3().y; }
  void setY3(double y) { CSVGPathMRBezier3To::setPoint3(CPoint2D(getX3(), y)); }
};

//---

class CQSVGPathClosePath : public CQSVGPathPart, public CSVGPathClosePath {
  Q_OBJECT

 public:
  CQSVGPathClosePath(CQSVG &svg, bool relative);

  CSVGPathPartType getType() const { return CSVGPathClosePath::getType(); }
};

#endif
