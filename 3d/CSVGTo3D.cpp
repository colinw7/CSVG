#include <CSVGTo3D.h>
#include <CSVG.h>
#include <CSVGRect.h>
#include <CSVGPolygon.h>
#include <CSVGPath.h>
#include <CSVGPathPart.h>

#include <CQGLControl.h>
#include <CGLTexture.h>
#include <CGLTessellation.h>
#include <CPoint3D.h>

#include <QApplication>

double       scale        { 1.0 };
double       dz           { 0.1 };
bool         print        { false };
bool         object       { false };
std::string  textureName;
CGLTexture  *texture      { nullptr };
uint         primitiveNum { 0 };
CVector3D    currentNormal;
CPoint2D     currentTexCoord;

CQGLWindow::ZData zdata;

CSVGBlock *block;

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  std::string filename;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-')  {
      auto arg = std::string(&argv[i][1]);

      if      (arg == "dz") {
        if (i < argc - 1)
          dz = atof(argv[++i]);
      }
      else if (arg == "texture") {
        if (i < argc - 1)
          textureName = argv[++i];
      }
      else if (arg == "print") {
        print = true;
      }
      else if (arg == "object") {
        object = true;
      }
    }
    else
      filename = argv[i];
  }

  if (filename == "")
    exit(1);

  CSVG svg(nullptr);

  svg.init();

  svg.setUniquify(true);
  svg.setQuiet(true);

  svg.read(filename);

  block = svg.getRootBlock();

  // scale to 0.0 -> 1.0
  scale = 1.0/std::max(svg.getWidth(), svg.getHeight());

  CQGLWindow window;

  if (! print && ! object) {
    window.show();

    return app.exec();
  }
  else {
    window.processSVGBlock(block);
  }
}

//---

CQGLWindow::
CQGLWindow() :
 QGLWidget()
{
  setFocusPolicy(Qt::StrongFocus);

  //-----

  control_ = new CQGLControl(this);

  auto *toolbar = control_->createToolBar();

  toolbar->setParent(this);

  connect(control_, SIGNAL(stateChanged()), this, SLOT(updateGL()));

  //-----

  resize(800, 800);
}

void
CQGLWindow::
resizeGL(int width, int height)
{
  w_ = width ;
  h_ = height;

  control_->handleResize(width, height);
}

void
CQGLWindow::
mousePressEvent(QMouseEvent *e)
{
  control_->handleMousePress(e);

  updateGL();
}

void
CQGLWindow::
mouseReleaseEvent(QMouseEvent *e)
{
  control_->handleMouseRelease(e);

  updateGL();
}

void
CQGLWindow::
mouseMoveEvent(QMouseEvent *e)
{
  control_->handleMouseMotion(e);

  updateGL();
}

void
CQGLWindow::
keyPressEvent(QKeyEvent *)
{
}

void
CQGLWindow::
initializeGL()
{
  glEnable(GL_COLOR_MATERIAL);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  if (textureName != "") {
    texture = new CGLTexture;

    texture->load(textureName);
  }
}

void
CQGLWindow::
paintGL()
{
  setupView();

  processSVGBlock(block);
}

void
CQGLWindow::
setupView()
{
  static GLfloat lpos1[4] = { 20.0,  20.0, 10.0, 1.0};
  static GLfloat lpos2[4] = {-20.0, -20.0, 10.0, 1.0};

  static float lambient [4] = {0.1, 0.1, 0.1, 0.0};
  static float ldiffuse1[4] = {0.6, 0.6, 0.6, 0.0};
  static float ldiffuse2[4] = {0.6, 0.6, 0.6, 0.0};

  glLightfv(GL_LIGHT0, GL_POSITION, lpos1);
  glLightfv(GL_LIGHT1, GL_POSITION, lpos2);

  glLightfv(GL_LIGHT0, GL_AMBIENT, lambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, ldiffuse1);

  glLightfv(GL_LIGHT1, GL_AMBIENT, lambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, ldiffuse2);

  glEnable(GL_NORMALIZE);
  //glEnable(GL_RESCALE_NORMALS);

  control_->updateGL();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
CQGLWindow::
processSVGBlock(CSVGBlock *block)
{
  CMatrix2D matrix;

  matrix.setIdentity();

  for (auto &child : block->children())
    processSVGObject(child, matrix);

  if (object) {
    std::cout << "Scene\n";

    for (uint i = 0; i < primitiveNum; ++i) {
      std::cout << "  Object Object" << i << "\n";
    }

    std::cout << "End\n";
  }
}

void
CQGLWindow::
processSVGObject(CSVGObject *object, const CMatrix2D &matrix)
{
  auto id = object->getObjTypeId();

  auto matrix1 = matrix*object->getTransform().getMatrix();

  auto scalePoint = [&](const CPoint2D &p) {
    return (scale*p)*matrix1;
  };

  if      (id == CSVGObjTypeId::RECT) {
    auto *rect = dynamic_cast<CSVGRect *>(object);

    // get bbox
    auto x1 = rect->getX().value();
    auto y1 = rect->getY().value();
    auto x2 = x1 + rect->getWidth ().value();
    auto y2 = y1 + rect->getHeight().value();

    Polygon poly;

    poly.points.push_back(scalePoint(CPoint2D(x1, y1)));
    poly.points.push_back(scalePoint(CPoint2D(x2, y1)));
    poly.points.push_back(scalePoint(CPoint2D(x2, y2)));
    poly.points.push_back(scalePoint(CPoint2D(x1, y2)));

    addPolygon(poly, 0.0);

    extrudePolygon(poly);

    addPolygon(poly, dz);
  }
  else if (id == CSVGObjTypeId::POLYGON) {
    auto *polygon = dynamic_cast<CSVGPolygon *>(object);

    Polygon poly;

    for (const auto &p : polygon->getPoints())
      poly.points.push_back(scalePoint(p));

    auto poly1 = tesselatePolygon(poly);

    addPolygon(poly1, 0.0);

    extrudePolygon(poly);

    addPolygon(poly1, dz);
  }
  else if (id == CSVGObjTypeId::PATH) {
    auto *path = dynamic_cast<CSVGPath *>(object);

    Polygon  poly;
    CPoint2D currentPoint(0, 0);

    const auto &parts = path->getPartList();

    for (const auto &part : parts.parts()) {
      auto type = part->getType();

      if      (type == CSVGPathPartType::MOVE_TO) {
        auto *moveTo = dynamic_cast<CSVGPathMoveTo *>(part);

        const auto &point = moveTo->getPoint();

        poly.points.push_back(scalePoint(point));

        currentPoint = point;
      }
      else if (type == CSVGPathPartType::LINE_TO) {
        auto *lineTo = dynamic_cast<CSVGPathLineTo *>(part);

        const auto &point = lineTo->getPoint();

        poly.points.push_back(scalePoint(point));

        currentPoint = point;
      }
      else if (type == CSVGPathPartType::RMOVE_TO) {
        auto *rmoveTo = dynamic_cast<CSVGPathRMoveTo *>(part);

        const auto &point = rmoveTo->getPoint();

        poly.points.push_back(scalePoint(point + currentPoint));

        currentPoint = point + currentPoint;
      }
      else if (type == CSVGPathPartType::RLINE_TO) {
        auto *lineTo = dynamic_cast<CSVGPathRLineTo *>(part);

        const auto &point = lineTo->getPoint();

        poly.points.push_back(scalePoint(point + currentPoint));

        currentPoint = point + currentPoint;
      }
      else if (type == CSVGPathPartType::RHLINE_TO) {
        auto *lineTo = dynamic_cast<CSVGPathRHLineTo *>(part);

        auto point = CPoint2D(lineTo->getDX(), 0);

        poly.points.push_back(scalePoint(point + currentPoint));

        currentPoint = point + currentPoint;
      }
      else if (type == CSVGPathPartType::RVLINE_TO) {
        auto *lineTo = dynamic_cast<CSVGPathRVLineTo *>(part);

        auto point = CPoint2D(0, lineTo->getDY());

        poly.points.push_back(scalePoint(point + currentPoint));

        currentPoint = point + currentPoint;
      }
      else if (type == CSVGPathPartType::ARC_TO) {
        auto *arcTo = dynamic_cast<CSVGPathArcTo *>(part);

        const auto &point2 = arcTo->getPoint2();

#if 0
        double xa = arcTo->getXA();
        int    fa = arcTo->getFA();
        int    fs = arcTo->getFS();

        double rx = arcTo->getRadiusX();
        double ry = arcTo->getRadiusY();

        bool unit_circle = false;

        double cx, cy, xr, yr, theta, delta;

        CSVGUtil::convertArcCoords(currentPoint.x, currentPoint.y, point2.x, point2.y,
                                   xa, rx, ry, fa, fs, unit_circle,
                                   &cx, &cy, &xr, &yr, &theta, &delta);
#endif

        poly.points.push_back(scalePoint(point2));

        currentPoint = point2;
      }
      else if (type == CSVGPathPartType::RARC_TO) {
        auto *arcTo = dynamic_cast<CSVGPathRArcTo *>(part);

        auto point2 = currentPoint + arcTo->getPoint2();

#if 0
        double xa = arcTo->getXA();
        int    fa = arcTo->getFA();
        int    fs = arcTo->getFS();

        double rx = arcTo->getRadiusX();
        double ry = arcTo->getRadiusY();

        bool unit_circle = false;

        double cx, cy, xr, yr, theta, delta;

        CSVGUtil::convertArcCoords(currentPoint.x, currentPoint.y, point2.x, point2.y,
                                   xa, rx, ry, fa, fs, unit_circle,
                                   &cx, &cy, &xr, &yr, &theta, &delta);
#endif

        poly.points.push_back(scalePoint(point2));

        currentPoint = point2;
      }
      else if (type == CSVGPathPartType::BEZIER2_TO) {
        auto *bezierTo = dynamic_cast<CSVGPathBezier2To *>(part);

        auto point1 = bezierTo->getPoint1();
        auto point2 = bezierTo->getPoint2();

        C2Bezier2D bezier(currentPoint, point1, point2);

        std::vector<CPoint2D> bpoints;

        CMathGeom2D::BezierToLines(bezier, bpoints, 0.5);

        uint n = bpoints.size();

        for (uint i = 1; i < n; ++i)
          poly.points.push_back(scalePoint(bpoints[i]));

        currentPoint = point2;
      }
      else if (type == CSVGPathPartType::RBEZIER2_TO) {
        auto *bezierTo = dynamic_cast<CSVGPathRBezier2To *>(part);

        auto point1 = currentPoint + bezierTo->getPoint1();
        auto point2 = currentPoint + bezierTo->getPoint2();

        C2Bezier2D bezier(currentPoint, point1, point2);

        std::vector<CPoint2D> bpoints;

        CMathGeom2D::BezierToLines(bezier, bpoints, 0.1);

        uint n = bpoints.size();

        for (uint i = 1; i < n; ++i)
          poly.points.push_back(scalePoint(bpoints[i]));

        currentPoint = point2;
      }
      else if (type == CSVGPathPartType::BEZIER3_TO) {
        auto *bezierTo = dynamic_cast<CSVGPathBezier3To *>(part);

        auto point1 = bezierTo->getPoint1();
        auto point2 = bezierTo->getPoint2();
        auto point3 = bezierTo->getPoint3();

        C3Bezier2D bezier(currentPoint, point1, point2, point3);

        std::vector<CPoint2D> bpoints;

        CMathGeom2D::BezierToLines(bezier, bpoints, 0.1);

        uint n = bpoints.size();

        for (uint i = 1; i < n; ++i)
          poly.points.push_back(scalePoint(bpoints[i]));

        currentPoint = point3;
      }
      else if (type == CSVGPathPartType::RBEZIER3_TO) {
        auto *bezierTo = dynamic_cast<CSVGPathRBezier3To *>(part);

        auto point1 = currentPoint + bezierTo->getPoint1();
        auto point2 = currentPoint + bezierTo->getPoint2();
        auto point3 = currentPoint + bezierTo->getPoint3();

        C3Bezier2D bezier(currentPoint, point1, point2, point3);

        std::vector<CPoint2D> bpoints;

        CMathGeom2D::BezierToLines(bezier, bpoints, 0.1);

        uint n = bpoints.size();

        for (uint i = 1; i < n; ++i)
          poly.points.push_back(scalePoint(bpoints[i]));

        currentPoint = point3;
      }
      else if (type == CSVGPathPartType::CLOSE_PATH) {
        poly.points.push_back(poly.points.front());
      }
      else
        std::cerr << "Unhandled Path Type\n";
    }

    auto poly1 = tesselatePolygon(poly);

    addPolygon(poly1, 0.0);

    extrudePolygon(poly);

    addPolygon(poly1, dz);
  }

  for (auto &child : object->children())
    processSVGObject(child, matrix1);
}

CQGLWindow::Polygon
CQGLWindow::
tesselatePolygon(const Polygon &poly) const
{
  double xmin, ymin, xmax, ymax;
  polyRange(poly, xmin, ymin, xmax, ymax);

  CGLTesselation tess;

  CGLTesselation::TPoints tpoints;

  for (auto &p : poly.points) {
    auto p1 = CPoint3D(p.p.x, p.p.y, 0.0);

    double tx = (p1.x - xmin)/(xmax - xmin);
    double ty = (p1.y - ymin)/(ymax - ymin);

    tpoints.emplace_back(p1, CPoint2D(tx, ty));
  }

  auto tpoints1 = tess.tesselate(tpoints);

  Polygon poly1;

  for (const auto &p1 : tpoints1)
    poly1.points.emplace_back(CPoint2D(p1.pv.x, p1.pv.y));

  return poly1;
}

void
CQGLWindow::
addPolygon(Polygon &poly, double z)
{
  //std::cerr << polyOrientStr(poly) << "\n";

  if (object) {
    std::cout << "Primitive Primitive" << primitiveNum << "\n";
    std::cout << "  Points\n";

    int ind { 0 };

    for (auto &p : poly.points) {
      p.ind = ind++;

      std::cout << "    " << p.p.x << " " << p.p.y << " " << z << "\n";
    }

    std::cout << "  End\n";

    std::cout << "  Faces\n";

    int nt = poly.points.size()/3;

    for (int i = 0; i < nt; ++i) {
      std::cout << "   ";
      std::cout << " " << poly.points[3*i + 0].ind + 1;
      std::cout << " " << poly.points[3*i + 1].ind + 1;
      std::cout << " " << poly.points[3*i + 2].ind + 1;
      std::cout << "\n";
    }

    std::cout << "  End\n";

    std::cout << "End\n";

    std::cout << "Object Object" << primitiveNum << "\n";
    std::cout << "  Primitive Primitive" << primitiveNum << "\n";
    std::cout << "End\n";

    ++primitiveNum;
  }
}

void
CQGLWindow::
extrudePolygon(Polygon &poly)
{
  //std::cerr << polyOrientStr(poly) << "\n";

  zdata.z[0] = 0.0;
  zdata.z[1] = dz;

  zdata.np = poly.points.size();

  //---

  if (object) {
    std::cout << "Primitive Primitive" << primitiveNum << "\n";
    std::cout << "  Points\n";

    int ind { 0 };

    for (auto &p : poly.points) {
      p.ind = ind++;

      std::cout << "    " << p.p.x << " " << p.p.y << " " << zdata.z[0] << "\n";
    }

    for (auto &p : poly.points) {
      std::cout << "    " << p.p.x << " " << p.p.y << " " << zdata.z[1] << "\n";
    }

    std::cout << "  End\n";
  }

  //---

  double xmin, ymin, xmax, ymax;
  polyRange(poly, xmin, ymin, xmax, ymax);

  //---

  uint n = poly.points.size();

  std::vector<LineData> lines;

  uint i1 = n - 1;

  for (uint i = 0; i < n; i1 = i++)
    lines.push_back(LineData(poly.points[i1], poly.points[i]));

  //------

  if (! print && ! object) {
    CGLTesselation tess;

    //tess.setWindingRule(GLU_TESS_WINDING_POSITIVE);

    if (texture) {
      glEnable(GL_TEXTURE_2D);

      glBindTexture(GL_TEXTURE_2D, texture->getId());
    }

    //------

    auto normal = CVector3D(0, 0, -1);

    execGlNormal(normal);

    tess.beginPolygon();
    tess.beginContour(2*n);

    //execGlBeginPolygon();

    for (uint i = 0; i < n; ++i) {
      double tx = (poly.points[i].p.x - xmin)/(xmax - xmin);
      double ty = (poly.points[i].p.y - ymin)/(ymax - ymin);

      tess.renderPoint(poly.points[i].p.x, poly.points[i].p.y, zdata.z[0], tx, ty);

    //execGlTexCoord(CPoint2D(tx, ty));
    //execGlVertex(poly.points[i], 0);
    }

    tess.endContour();
    tess.endPolygon();

    //execGlEnd();

    //------

    if (texture)
      glDisable(GL_TEXTURE_2D);
  }

  //------

  if (object) {
    std::cout << "  Faces\n";
  }

  // extrude polygon per edge (line)
  uint nl = lines.size();

  for (uint i = 0; i < nl; ++i) {
    if (object)
      std::cout << "    ";

    const auto &line = lines[i];

    const auto &p1 = line.p1;
    const auto &p2 = line.p2;

    auto dx = p2.p.x - p1.p.x;
    auto dy = p2.p.y - p1.p.y;

    auto a = std::atan2(dy, dx);

    auto normal = CVector3D(sin(a), cos(a), 0.0);

#if 0
    bool flip = false;

    {
    Polygon poly1;

    if (std::abs(dx) >= std::abs(dy)) {
      poly1.points.push_back(PointData(p1.p.x, 0));
      poly1.points.push_back(PointData(p2.p.x, 0));
      poly1.points.push_back(PointData(p2.p.x, dz));
      poly1.points.push_back(PointData(p1.p.x, dz));
    }
    else {
      poly1.points.push_back(PointData(p1.p.y, 0));
      poly1.points.push_back(PointData(p2.p.y, 0));
      poly1.points.push_back(PointData(p2.p.y, dz));
      poly1.points.push_back(PointData(p1.p.y, dz));
    }

    //std::cerr << polyOrientStr(poly1) << "\n";

    if (polyOrient(poly1) == CPOLYGON_ORIENTATION_ANTICLOCKWISE)
      flip = true;
    }

    execGlBeginPolygon();

    execGlNormal(normal);

    if (! flip) {
      execGlTexCoord(CPoint2D(0, 0)); execGlVertex(p1, 0);
      execGlTexCoord(CPoint2D(0, 1)); execGlVertex(p2, 0);
      execGlTexCoord(CPoint2D(1, 1)); execGlVertex(p2, 1);
      execGlTexCoord(CPoint2D(1, 0)); execGlVertex(p1, 1);
    }
    else {
      execGlTexCoord(CPoint2D(0, 0)); execGlVertex(p2, 0);
      execGlTexCoord(CPoint2D(0, 1)); execGlVertex(p1, 0);
      execGlTexCoord(CPoint2D(1, 1)); execGlVertex(p1, 1);
      execGlTexCoord(CPoint2D(1, 0)); execGlVertex(p2, 1);
    }
    execGlEnd();

#else
    execGlBeginPolygon();

    execGlNormal(normal);

    execGlTexCoord(CPoint2D(0, 0)); execGlVertex(p1, 0);
    execGlTexCoord(CPoint2D(0, 1)); execGlVertex(p2, 0);
    execGlTexCoord(CPoint2D(1, 1)); execGlVertex(p2, 1);
    execGlTexCoord(CPoint2D(1, 0)); execGlVertex(p1, 1);

    execGlEnd();
#endif

    if (object)
      std::cout << "\n";
  }

  if (object)
    std::cout << "  End\n";

  //------

  if (! print && ! object) {
    CGLTesselation tess;

    if (texture) {
      glEnable(GL_TEXTURE_2D);

      glBindTexture(GL_TEXTURE_2D, texture->getId());
    }

    //------

    auto normal = CVector3D(0, 0, 1);

    execGlNormal(normal);

    tess.beginPolygon();
    tess.beginContour(2*n);

    //execGlBeginPolygon();

    for (uint i = 0; i < n; ++i) {
      int i1 = n - 1 - i;

      double tx = (poly.points[i1].p.x - xmin)/(xmax - xmin);
      double ty = (poly.points[i1].p.y - ymin)/(ymax - ymin);

      tess.renderPoint(poly.points[i1].p.x, poly.points[i1].p.y, zdata.z[1], tx, ty);

    //execGlTexCoord(CPoint2D(tx, ty));
    //execGlVertex(poly.points[i1], 1);
    }

    tess.endContour();
    tess.endPolygon();

    //execGlEnd();

    //------

    if (texture)
      glDisable(GL_TEXTURE_2D);
  }

  //---

  if (object) {
    std::cout << "End\n";

    std::cout << "Object Object" << primitiveNum << "\n";
    std::cout << "  Primitive Primitive" << primitiveNum << "\n";
    std::cout << "End\n";

    ++primitiveNum;
  }
}

int
CQGLWindow::
polyOrient(const Polygon &poly) const
{
  std::vector<CPoint2D> points;

  for (const auto &p : poly.points)
    points.push_back(p.p);

  return CMathGeom2D::PolygonOrientation(points);
}

void
CQGLWindow::
polyRange(const Polygon &poly, double &xmin, double &ymin, double &xmax, double &ymax) const
{
  uint n = poly.points.size();

  xmin = poly.points[0].p.x;
  ymin = poly.points[0].p.y;
  xmax = xmin;
  ymax = ymin;

  for (uint i = 1; i < n; ++i) {
    xmin = std::min(xmin, poly.points[i].p.x);
    ymin = std::min(ymin, poly.points[i].p.y);
    xmax = std::max(xmax, poly.points[i].p.x);
    ymax = std::max(ymax, poly.points[i].p.y);
  }
}

const char *
CQGLWindow::
polyOrientStr(const Polygon &poly) const
{
  auto orient = polyOrient(poly);

  return (orient == CPOLYGON_ORIENTATION_CLOCKWISE ? "clockwise" : "anti-clockwise");
}

void
CQGLWindow::
execGlBeginPolygon()
{
  if      (print) {
    std::cout << "  glBegin(GL_POLYGON);\n";
  }
  else if (object) {
  }
  else {
    glBegin(GL_POLYGON);
  }
}

void
CQGLWindow::
execGlEnd()
{
  if      (print) {
    std::cout << "  glEnd();\n";
  }
  else if (object) {
  }
  else {
    glEnd();
  }
}

void
CQGLWindow::
execGlNormal(const CVector3D &normal)
{
  if      (print) {
    std::cout << "  glNormal3d(" <<
      normal.x() << ", " << normal.y() << ", " << normal.z() << ");\n";
  }
  else if (object) {
    currentNormal = normal;
  }
  else {
    glNormal3d(normal.x(), normal.y(), normal.z());
  }
}

void
CQGLWindow::
execGlTexCoord(const CPoint2D &t)
{
  if      (print) {
    std::cout << "  glTexCoord2d(" << t.x << ", " << t.y << ");\n";
  }
  else if (object) {
    currentTexCoord = t;
  }
  else {
    glTexCoord2d(0, 0);
  }
}

void
CQGLWindow::
execGlVertex(const PointData &p, int zind)
{
  if (print) {
    std::cout << "  glVertex3d(" << p.p.x << ", " << p.p.y << ", " << zdata.z[zind] << ");\n";
  }
  else if (object) {
    uint ind = (zind == 0 ? p.ind : p.ind + zdata.np);

    std::cout << ind + 1 << " ";
  }
  else {
    glVertex3d(p.p.x, p.p.y, zdata.z[zind]);
  }
}
