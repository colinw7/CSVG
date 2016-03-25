#ifndef CSVGPathPart_H
#define CSVGPathPart_H

enum class CSVGPathPartType {
  MOVE_TO,
  LINE_TO,
  RLINE_TO,
  HLINE_TO,
  VLINE_TO,
  ARC_TO,
  RARC_TO,
  BEZIER2_TO,
  RBEZIER2_TO,
  BEZIER3_TO,
  RBEZIER3_TO,
  CLOSE_PATH
};

class CSVGPathPart {
 protected:
  CSVG             &svg_;
  CSVGPathPartType  type_;

 public:
  CSVGPathPart(CSVG &svg, CSVGPathPartType type) :
   svg_(svg), type_(type) {
  }

  virtual ~CSVGPathPart() { }

  virtual void draw() = 0;

  CSVGPathPartType getType() const { return type_; }

  bool getBBox(CBBox2D &bbox);

  virtual void print(std::ostream &os) const = 0;
};

class CSVGPathMoveTo : public CSVGPathPart {
 public:
  CSVGPathMoveTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

class CSVGPathLineTo : public CSVGPathPart {
 public:
  CSVGPathLineTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

class CSVGPathRLineTo : public CSVGPathPart {
 public:
  CSVGPathRLineTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point_;
};

class CSVGPathHLineTo : public CSVGPathPart {
 public:
  CSVGPathHLineTo(CSVG &svg, double d);

  double getLength() const { return d_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  double d_;
};

class CSVGPathVLineTo : public CSVGPathPart {
 public:
  CSVGPathVLineTo(CSVG &svg, double d);

  double getLength() const { return d_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  double d_;
};

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

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  double   rx_, ry_, xa_;
  int      fa_, fs_;
  CPoint2D point2_;
};

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

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  double   rx_, ry_, xa_;
  int      fa_, fs_;
  CPoint2D point2_;
};

class CSVGPathBezier2To : public CSVGPathPart {
 public:
  CSVGPathBezier2To(CSVG &svg, double x1, double y1, double x2, double y2);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point1_, point2_;
};

class CSVGPathRBezier2To : public CSVGPathPart {
 public:
  CSVGPathRBezier2To(CSVG &svg, double x1, double y1, double x2, double y2);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point1_, point2_;
};

class CSVGPathBezier3To : public CSVGPathPart {
 public:
  CSVGPathBezier3To(CSVG &svg, double x1, double y1,
                    double x2, double y2, double x3, double y3);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }
  const CPoint2D &getPoint3() const { return point3_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point1_, point2_, point3_;
};

class CSVGPathRBezier3To : public CSVGPathPart {
 public:
  CSVGPathRBezier3To(CSVG &svg, double x1, double y1,
                     double x2, double y2, double x3, double y3);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }
  const CPoint2D &getPoint3() const { return point3_; }

  void draw() override;

  void print(std::ostream &os) const override;

 private:
  CPoint2D point1_, point2_, point3_;
};

class CSVGPathClosePath : public CSVGPathPart {
 public:
  CSVGPathClosePath(CSVG &svg);

  void draw() override;

  void print(std::ostream &os) const override;
};

#endif
