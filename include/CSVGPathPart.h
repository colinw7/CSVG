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

  //bool getBBox(CBBox2D &bbox);

  virtual void moveBy(const CVector2D &d) = 0;

  virtual void draw() = 0;

  virtual double getLength(const CPoint2D &) const;

  virtual CPoint2D getEndPoint(const CPoint2D &p) const;

  virtual bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
                      CPoint2D &pi, double &a) const;

  virtual void print(std::ostream &os) const = 0;

 protected:
  CSVG&            svg_;
  CSVGPathPartType type_;
};

//---

class CSVGPathPartList {
 public:
  typedef std::vector<CSVGPathPart *> Parts;

 public:
  CSVGPathPartList() { }

  const Parts &parts() const { return parts_; }

  void push_back(CSVGPathPart *part) {
    parts_.push_back(part);

    invalidate();
  }

  bool empty() const {
    return parts_.empty();
  }

  void draw(CSVGBuffer *buffer, std::vector<CPoint2D> &points) const;

  double getLength() const;

  bool interp(double s, double *xi, double *yi, double *a, int *pi) const;

  bool getBBox(CSVGBuffer *buffer, CBBox2D &bbox) const;

  void moveBy(const CVector2D &d);

  void print(std::ostream &os) const;

  void invalidate() {
    length_.setInvalid();
  }

 private:
  Parts            parts_;
  mutable COptReal length_;
};

//---

class CSVGPathMoveTo : public CSVGPathPart {
 public:
  CSVGPathMoveTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  double getLength(const CPoint2D &) const override { return 0; }

  CPoint2D getEndPoint(const CPoint2D &) const override { return point_; }

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

//---

class CSVGPathRMoveTo : public CSVGPathPart {
 public:
  CSVGPathRMoveTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  double getLength(const CPoint2D &) const override { return 0; }

  CPoint2D getEndPoint(const CPoint2D &) const override { return point_; }

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

//---

class CSVGPathLineTo : public CSVGPathPart {
 public:
  CSVGPathLineTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

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

  const CPoint2D &getPoint() const { return point_; }

  void moveBy(const CVector2D &d) override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

//---

class CSVGPathHLineTo : public CSVGPathPart {
 public:
  CSVGPathHLineTo(CSVG &svg, double d);

  double getDistance() const { return d_; }

  double getLength(const CPoint2D &) const override { return d_; }

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + CPoint2D(d_, 0); }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  double d_ { 0 };
};

//---

class CSVGPathVLineTo : public CSVGPathPart {
 public:
  CSVGPathVLineTo(CSVG &svg, double d);

  double getDistance() const { return d_; }

  double getLength(const CPoint2D &) const override { return d_; }

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + CPoint2D(0, d_); }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  double d_ { 0 };
};

//---

class CSVGPathArcTo : public CSVGPathPart {
 public:
  CSVGPathArcTo(CSVG &svg, double rx, double ry, double xa,
                int fa, int fs, double x2, double y2);

  double getRadiusX() const { return rx_; }
  double getRadiusY() const { return ry_; }

  double getXA() const { return xa_; }

  int getFA() const { return fa_; }
  int getFS() const { return fs_; }

  const CPoint2D &getPoint2() const { return point2_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

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
  CSVGPathRArcTo(CSVG &svg, double rx, double ry, double xa,
                 int fa, int fs, double x2, double y2);

  double getRadiusX() const { return rx_; }
  double getRadiusY() const { return ry_; }

  double getXA() const { return xa_; }

  int getFA() const { return fa_; }
  int getFS() const { return fs_; }

  const CPoint2D &getPoint2() const { return point2_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point2_; }

  void print(std::ostream &os) const override;

 private:
  double   rx_ { 0 };
  double   ry_ { 0 };
  double   xa_ { 0 };
  int      fa_ { 0 };
  int      fs_ { 0 };
  CPoint2D point2_;
};

//---

class CSVGPathBezier2To : public CSVGPathPart {
 public:
  CSVGPathBezier2To(CSVG &svg, double x1, double y1, double x2, double y2);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  CPoint2D getEndPoint(const CPoint2D &) const override { return point2_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point1_;
  CPoint2D point2_;
};

//---

class CSVGPathRBezier2To : public CSVGPathPart {
 public:
  CSVGPathRBezier2To(CSVG &svg, double x1, double y1, double x2, double y2);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point2_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point1_;
  CPoint2D point2_;
};

//---

class CSVGPathBezier3To : public CSVGPathPart {
 public:
  CSVGPathBezier3To(CSVG &svg, double x1, double y1,
                    double x2, double y2, double x3, double y3);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }
  const CPoint2D &getPoint3() const { return point3_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &) const override { return point3_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point1_;
  CPoint2D point2_;
  CPoint2D point3_;

  mutable COptReal length_;
};

//---

class CSVGPathRBezier3To : public CSVGPathPart {
 public:
  CSVGPathRBezier3To(CSVG &svg, double x1, double y1,
                     double x2, double y2, double x3, double y3);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }
  const CPoint2D &getPoint3() const { return point3_; }

  void moveBy(const CVector2D &d) override;

  void draw() override;

  double getLength(const CPoint2D &p) const override;

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p + point3_; }

  bool interp(double s, const CPoint2D &p1, const CPoint2D &p2,
              CPoint2D &pi, double &a) const override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point1_;
  CPoint2D point2_;
  CPoint2D point3_;

  mutable COptReal length_;
};

//---

class CSVGPathClosePath : public CSVGPathPart {
 public:
  CSVGPathClosePath(CSVG &svg);

  void moveBy(const CVector2D &d) override;

  void draw() override;

  double getLength(const CPoint2D &) const override { return 0; }

  CPoint2D getEndPoint(const CPoint2D &p) const override { return p; }

  void print(std::ostream &os) const override;
};

#endif
