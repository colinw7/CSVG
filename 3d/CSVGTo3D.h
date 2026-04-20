#include <QWidget>
#include <QGLWidget>

#include <CVector3D.h>
#include <CLine2D.h>
#include <CMatrix2D.h>

#include <vector>

class CQGLControl;
class CSVGBlock;
class CSVGObject;

class CQGLWindow : public QGLWidget {
  Q_OBJECT

 public:
  struct PointData {
    PointData() { }
    PointData(double x, double y) : p(x, y) { }
    PointData(const CPoint2D &p1) : p(p1) { }

    CPoint2D p;
    double   z { 0.0 };
    int      ind { 0 };
  };

  struct LineData {
    LineData() { }
    LineData(const PointData &p11, const PointData &p21) :
     p1(p11), p2(p21) {
    }

    PointData p1;
    PointData p2;
  };

  struct ZData {
    double z[2] { 0.0, 1.0 };
    uint   ind { 0 };
    uint   np { 0 };
  };

  struct Polygon {
    std::vector<PointData> points;
  };

 public:
  CQGLWindow();

 private:
  void initializeGL() override;

  void resizeGL(int width, int height) override;

  void paintGL() override;

  void mousePressEvent  (QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent   (QMouseEvent *event) override;

  void keyPressEvent(QKeyEvent *e) override;

  void setupView();

 public:
  void processSVGBlock(CSVGBlock *block);
  void processSVGObject(CSVGObject *object, const CMatrix2D &matrix);

 private:
  Polygon tesselatePolygon(const Polygon &poly) const;

  void addPolygon(Polygon &poly, double z);
  void extrudePolygon(Polygon &polygon);

  int polyOrient(const Polygon &poly) const;
  const char *polyOrientStr(const Polygon &poly) const;

  void polyRange(const Polygon &poly, double &xmin, double &ymin,
                 double &xmax, double &ymax) const;

  void execGlBeginPolygon();
  void execGlEnd();
  void execGlNormal(const CVector3D &normal);
  void execGlTexCoord(const CPoint2D &t);
  void execGlVertex(const PointData &p, int zind);

 private:
  CQGLControl *control_ { nullptr };
  int          w_ { 100 };
  int          h_ { 100 };
};
