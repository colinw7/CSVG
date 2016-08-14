#ifndef CSVGPathPart_H
#define CSVGPathPart_H

#include <CSVGTypes.h>
#include <CBBox2D.h>
#include <COptVal.h>
#include <vector>

class CSVG;
class CSVGBuffer;

class CSVGPathPart {
 public:
  CSVGPathPart(CSVG &svg, CSVGPathPartType type) :
   svg_(svg), type_(type) {
  }

  virtual ~CSVGPathPart() { }

  CSVGPathPartType getType() const { return type_; }

  virtual std::string id() const = 0;

  //bool getBBox(CBBox2D &bbox);

  virtual void moveBy(const CVector2D &d) = 0;

  virtual void draw(CSVGBuffer *buffer) = 0;

  virtual double getLength(const CPoint2D &) const;

  virtual CPoint2D getEndPoint(const CPoint2D &p) const;

  virtual bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
                      CPoint2D &pi, double &a) const = 0;

  virtual void print(std::ostream &os) const = 0;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPathPart &part) {
    part.print(os);

    return os;
  }

  static std::string partTypeName(CSVGPathPartType type);

 protected:
  CSVG&            svg_;
  CSVGPathPartType type_;
};

//---

class CSVGPathPartList {
 public:
  typedef std::vector<CSVGPathPart *> Parts;

 public:
  CSVGPathPartList();

 ~CSVGPathPartList();

  const Parts &parts() const { return parts_; }

  void push_back(CSVGPathPart *part);

  bool empty() const { return parts_.empty(); }

  void draw(CSVGBuffer *buffer, std::vector<CPoint2D> &points, std::vector<double> &angles) const;

  double getLength() const;

  bool interp(double s, double *xi, double *yi, double *a, int *pi=0) const;

  bool getBBox(CSVGBuffer *buffer, CBBox2D &bbox) const;

  void moveBy(const CVector2D &d);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPathPartList &list) {
    list.print(os);

    return os;
  }

  void invalidate();

 private:
  Parts            parts_;
  mutable COptReal length_;
};

//---

class CSVGPathMoveTo : public CSVGPathPart {
 public:
  CSVGPathMoveTo(CSVG &svg, double x, double y);

  std::string id() const override { return "moveTo"; }

  const CPoint2D &getPoint() const { return point_; }
  void setPoint(const CPoint2D &p) { point_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &) const override { return 0; }

  CPoint2D getEndPoint(const CPoint2D &) const override { return point_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

//---

class CSVGPathRMoveTo : public CSVGPathPart {
 public:
  CSVGPathRMoveTo(CSVG &svg, double x, double y);

  std::string id() const override { return "rlineTo"; }

  const CPoint2D &getPoint() const { return point_; }
  void setPoint(const CPoint2D &p) { point_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &) const override { return 0; }

  CPoint2D getEndPoint(const CPoint2D &) const override { return point_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

//---

class CSVGPathLineTo : public CSVGPathPart {
 public:
  CSVGPathLineTo(CSVG &svg, double x, double y);

  std::string id() const override { return "lineTo"; }

  const CPoint2D &getPoint() const { return point_; }
  void setPoint(const CPoint2D &p) { point_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &) const override { return point_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

//---

class CSVGPathRLineTo : public CSVGPathPart {
 public:
  CSVGPathRLineTo(CSVG &svg, double x, double y);

  std::string id() const override { return "rlineTo"; }

  const CPoint2D &getPoint() const { return point_; }
  void setPoint(const CPoint2D &p) { point_ = p; }

  void moveBy(const CVector2D &d) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point_; }

  void draw(CSVGBuffer *buffer) override;

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

//---

class CSVGPathHLineTo : public CSVGPathPart {
 public:
  CSVGPathHLineTo(CSVG &svg, double x);

  std::string id() const override { return "hlineTo"; }

  double getX() const { return x_; }
  void setX(double r) { x_ = r; }

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return CPoint2D(x_, p.y); }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  double x_ { 0 };
};

//---

class CSVGPathRHLineTo : public CSVGPathPart {
 public:
  CSVGPathRHLineTo(CSVG &svg, double dx);

  std::string id() const override { return "rhlineTo"; }

  double getDX() const { return dx_; }
  void setDX(double r) { dx_ = r; }

  double getLength(const CPoint2D &) const override { return fabs(dx_); }

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + CPoint2D(dx_, 0); }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  double dx_ { 0 };
};

//---

class CSVGPathVLineTo : public CSVGPathPart {
 public:
  CSVGPathVLineTo(CSVG &svg, double y);

  std::string id() const override { return "vlineTo"; }

  double getY() const { return y_; }
  void setY(double r) { y_ = r; }

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + CPoint2D(p.x, y_); }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  double y_ { 0 };
};

//---

class CSVGPathRVLineTo : public CSVGPathPart {
 public:
  CSVGPathRVLineTo(CSVG &svg, double dy);

  std::string id() const override { return "rvlineTo"; }

  double getDY() const { return dy_; }
  void setDY(double r) { dy_ = r; }

  double getLength(const CPoint2D &) const override { return fabs(dy_); }

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + CPoint2D(0, dy_); }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  double dy_ { 0 };
};

//---

class CSVGPathArcTo : public CSVGPathPart {
 public:
  CSVGPathArcTo(CSVG &svg, double rx, double ry, double xa, int fa, int fs, double x2, double y2);

  std::string id() const override { return "arcTo"; }

  double getRadiusX() const { return rx_; }
  void setRadiusX(double r) { rx_ = r; }

  double getRadiusY() const { return ry_; }
  void setRadiusY(double r) { ry_ = r; }

  double getXA() const { return xa_; }
  void setXA(double x) { xa_ = x; }

  int getFA() const { return fa_; }
  void setFA(double f) { fa_ = f; }

  int getFS() const { return fs_; }
  void setFS(double f) { fs_ = f; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &) const override { return point2_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

  void invalidate() {
    length_.setInvalid();
  }

 private:
  double   rx_ { 0 };
  double   ry_ { 0 };
  double   xa_ { 0 };
  int      fa_ { 0 };
  int      fs_ { 0 };
  CPoint2D point2_;

  mutable COptReal length_;
};

//---

class CSVGPathRArcTo : public CSVGPathPart {
 public:
  CSVGPathRArcTo(CSVG &svg, double rx, double ry, double xa, int fa, int fs, double x2, double y2);

  std::string id() const override { return "rarcTo"; }

  double getRadiusX() const { return rx_; }
  void setRadiusX(double r) { rx_ = r; }

  double getRadiusY() const { return ry_; }
  void setRadiusY(double r) { ry_ = r; }

  double getXA() const { return xa_; }
  void setXA(double x) { xa_ = x; }

  int getFA() const { return fa_; }
  void setFA(double f) { fa_ = f; }

  int getFS() const { return fs_; }
  void setFS(double f) { fs_ = f; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point2_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  double   rx_ { 0 };
  double   ry_ { 0 };
  double   xa_ { 0 };
  int      fa_ { 0 };
  int      fs_ { 0 };
  CPoint2D point2_;

  mutable COptReal length_;
};

//---

class CSVGPathBezier2To : public CSVGPathPart {
 public:
  CSVGPathBezier2To(CSVG &svg, double x1, double y1, double x2, double y2);

  std::string id() const override { return "bezier2To"; }

  const CPoint2D &getPoint1() const { return point1_; }
  void setPoint1(const CPoint2D &p) { point1_ = p; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &) const override { return point2_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D         point1_;
  CPoint2D         point2_;
  mutable COptReal length_;
};

//---

class CSVGPathMBezier2To : public CSVGPathPart {
 public:
  CSVGPathMBezier2To(CSVG &svg, double x2, double y2);

  std::string id() const override { return "mbezier2To"; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &) const override { return point2_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D         point1_;
  CPoint2D         point2_;
  mutable COptReal length_;
};

//---

class CSVGPathRBezier2To : public CSVGPathPart {
 public:
  CSVGPathRBezier2To(CSVG &svg, double x1, double y1, double x2, double y2);

  std::string id() const override { return "rbezier2To"; }

  const CPoint2D &getPoint1() const { return point1_; }
  void setPoint1(const CPoint2D &p) { point1_ = p; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point2_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D         point1_;
  CPoint2D         point2_;
  mutable COptReal length_;
};

//---

class CSVGPathMRBezier2To : public CSVGPathPart {
 public:
  CSVGPathMRBezier2To(CSVG &svg, double x2, double y2);

  std::string id() const override { return "mrbezier2To"; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point2_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D         point1_;
  CPoint2D         point2_;
  mutable COptReal length_;
};

//---

class CSVGPathBezier3To : public CSVGPathPart {
 public:
  CSVGPathBezier3To(CSVG &svg, double x1, double y1, double x2, double y2, double x3, double y3);

  std::string id() const override { return "bezier3To"; }

  const CPoint2D &getPoint1() const { return point1_; }
  void setPoint1(const CPoint2D &p) { point1_ = p; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  const CPoint2D &getPoint3() const { return point3_; }
  void setPoint3(const CPoint2D &p) { point3_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &) const override { return point3_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D         point1_;
  CPoint2D         point2_;
  CPoint2D         point3_;
  mutable COptReal length_;
};

//---

class CSVGPathMBezier3To : public CSVGPathPart {
 public:
  CSVGPathMBezier3To(CSVG &svg, double x2, double y2, double x3, double y3);

  std::string id() const override { return "mbezier3To"; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  const CPoint2D &getPoint3() const { return point3_; }
  void setPoint3(const CPoint2D &p) { point3_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &) const override { return point3_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D         point1_;
  CPoint2D         point2_;
  CPoint2D         point3_;
  mutable COptReal length_;
};

//---

class CSVGPathRBezier3To : public CSVGPathPart {
 public:
  CSVGPathRBezier3To(CSVG &svg, double x1, double y1, double x2, double y2, double x3, double y3);

  std::string id() const override { return "rbezier3To"; }

  const CPoint2D &getPoint1() const { return point1_; }
  void setPoint1(const CPoint2D &p) { point1_ = p; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  const CPoint2D &getPoint3() const { return point3_; }
  void setPoint3(const CPoint2D &p) { point3_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point3_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D         point1_;
  CPoint2D         point2_;
  CPoint2D         point3_;
  mutable COptReal length_;
};

//---

class CSVGPathMRBezier3To : public CSVGPathPart {
 public:
  CSVGPathMRBezier3To(CSVG &svg, double x2, double y2, double x3, double y3);

  std::string id() const override { return "mrbezier3To"; }

  const CPoint2D &getPoint2() const { return point2_; }
  void setPoint2(const CPoint2D &p) { point2_ = p; }

  const CPoint2D &getPoint3() const { return point3_; }
  void setPoint3(const CPoint2D &p) { point3_ = p; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point3_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D         point1_;
  CPoint2D         point2_;
  CPoint2D         point3_;
  mutable COptReal length_;
};

//---

class CSVGPathClosePath : public CSVGPathPart {
 public:
  CSVGPathClosePath(CSVG &svg, bool relative);

  std::string id() const override { return "closePath"; }

  void moveBy(const CVector2D &d) override;

  void draw(CSVGBuffer *buffer) override;

  double getLength(const CPoint2D &) const override { return 0; }

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  bool relative_ { false };
};

#endif
