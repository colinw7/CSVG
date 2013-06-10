enum CSVGPathPartType {
  CSVG_PATH_PART_MOVE_TO,
  CSVG_PATH_PART_LINE_TO,
  CSVG_PATH_PART_RLINE_TO,
  CSVG_PATH_PART_HLINE_TO,
  CSVG_PATH_PART_VLINE_TO,
  CSVG_PATH_PART_ARC_TO,
  CSVG_PATH_PART_RARC_TO,
  CSVG_PATH_PART_BEZIER2_TO,
  CSVG_PATH_PART_RBEZIER2_TO,
  CSVG_PATH_PART_BEZIER3_TO,
  CSVG_PATH_PART_RBEZIER3_TO,
  CSVG_PATH_PART_CLOSE_PATH
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

  void printp(std::ostream *os) const { print(*os); }
};

class CSVGPathMoveTo : public CSVGPathPart {
 private:
  CPoint2D point_;

 public:
  CSVGPathMoveTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathLineTo : public CSVGPathPart {
 private:
  CPoint2D point_;

 public:
  CSVGPathLineTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathRLineTo : public CSVGPathPart {
 private:
  CPoint2D point_;

 public:
  CSVGPathRLineTo(CSVG &svg, double x, double y);

  const CPoint2D &getPoint() const { return point_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathHLineTo : public CSVGPathPart {
 private:
  double d_;

 public:
  CSVGPathHLineTo(CSVG &svg, double d);

  double getLength() const { return d_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathVLineTo : public CSVGPathPart {
 private:
  double d_;

 public:
  CSVGPathVLineTo(CSVG &svg, double d);

  double getLength() const { return d_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathArcTo : public CSVGPathPart {
 private:
  double   rx_, ry_, xa_;
  int      fa_, fs_;
  CPoint2D point2_;

 public:
  CSVGPathArcTo(CSVG &svg, double rx, double ry, double xa,
                int fa, int fs, double x2, double y2);

  double getRadiusX() const { return rx_; }
  double getRadiusY() const { return ry_; }

  double getXA() const { return xa_; }

  int getFA() const { return fa_; }
  int getFS() const { return fs_; }

  const CPoint2D &getPoint2() const { return point2_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathRArcTo : public CSVGPathPart {
 private:
  double   rx_, ry_, xa_;
  int      fa_, fs_;
  CPoint2D point2_;

 public:
  CSVGPathRArcTo(CSVG &svg, double rx, double ry, double xa,
                 int fa, int fs, double x2, double y2);

  double getRadiusX() const { return rx_; }
  double getRadiusY() const { return ry_; }

  double getXA() const { return xa_; }

  int getFA() const { return fa_; }
  int getFS() const { return fs_; }

  const CPoint2D &getPoint2() const { return point2_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathBezier2To : public CSVGPathPart {
 private:
  CPoint2D point1_, point2_;

 public:
  CSVGPathBezier2To(CSVG &svg, double x1, double y1, double x2, double y2);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathRBezier2To : public CSVGPathPart {
 private:
  CPoint2D point1_, point2_;

 public:
  CSVGPathRBezier2To(CSVG &svg, double x1, double y1, double x2, double y2);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathBezier3To : public CSVGPathPart {
 private:
  CPoint2D point1_, point2_, point3_;

 public:
  CSVGPathBezier3To(CSVG &svg, double x1, double y1,
                    double x2, double y2, double x3, double y3);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }
  const CPoint2D &getPoint3() const { return point3_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathRBezier3To : public CSVGPathPart {
 private:
  CPoint2D point1_, point2_, point3_;

 public:
  CSVGPathRBezier3To(CSVG &svg, double x1, double y1,
                     double x2, double y2, double x3, double y3);

  const CPoint2D &getPoint1() const { return point1_; }
  const CPoint2D &getPoint2() const { return point2_; }
  const CPoint2D &getPoint3() const { return point3_; }

  void draw();

  void print(std::ostream &os) const;
};

class CSVGPathClosePath : public CSVGPathPart {
 public:
  CSVGPathClosePath(CSVG &svg);

  void draw();

  void print(std::ostream &os) const;
};
