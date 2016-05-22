#include <CSVGPathPart.h>
#include <CSVG.h>
#include <CSVGBuffer.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>
#include <CStrParse.h>
#include <C2Bezier2D.h>
#include <C3Bezier2D.h>
#include <CMathGeom2D.h>
#include <CArcToBezier.h>

bool
CSVGPathPart::
interp(double s, const CPoint2D &p1, const CPoint2D &p2, CPoint2D &pi, double &a) const
{
std::cerr << "CSVGPathPart::interp" << std::endl;
  double xi = CSVGUtil::map(s, 0, 1, p1.x, p2.x);
  double yi = CSVGUtil::map(s, 0, 1, p1.y, p2.y);

  pi = CPoint2D(xi, yi);
  a  = CMathGen::atan2(p2.x - p1.x, p2.y - p1.y);

  return true;
}

double
CSVGPathPart::
getLength(const CPoint2D &) const
{
std::cerr << "CSVGPathPart::getLength" << std::endl;
  return 0;
}

CPoint2D
CSVGPathPart::
getEndPoint(const CPoint2D &p) const
{
std::cerr << "CSVGPathPart::getEndPoint" << std::endl;
  return p;
}

//------

void
CSVGPathPartList::
draw(CSVGBuffer *buffer, std::vector<CPoint2D> &points) const
{
  auto p = parts_.begin();

  double x1, y1;

  while (p != parts_.end() && ! buffer->pathGetCurrentPoint(&x1, &y1)) {
    (*p)->draw(buffer);

    ++p;
  }

  if (p == parts_.end())
    return;

  points.push_back(CPoint2D(x1, y1));

  while (p != parts_.end()) {
    (*p)->draw(buffer);

    ++p;

    double x2, y2;

    buffer->pathGetCurrentPoint(&x2, &y2);

    if (! REAL_EQ(x1, x2) || ! REAL_EQ(y1, y2)) {
      points.push_back(CPoint2D(x2, y2));

      x1 = x2;
      y1 = y2;
    }
  }
}

double
CSVGPathPartList::
getLength() const
{
  if (length_.isValid())
    return length_.getValue();

  //---

  auto ppart = parts_.begin();

  CPoint2D p(0, 0);

  for ( ; ppart != parts_.end(); ++ppart) {
    if ((*ppart)->getType() == CSVGPathPartType::MOVE_TO ||
        (*ppart)->getType() == CSVGPathPartType::RMOVE_TO) {
      p = (*ppart)->getEndPoint(p);
      break;
    }
  }

  double l = 0;

  for ( ; ppart != parts_.end(); ++ppart) {
    l += (*ppart)->getLength(p);
    p  = (*ppart)->getEndPoint(p);
  }

  //---

  length_ = l;

  return l;
}

bool
CSVGPathPartList::
interp(double s, double *xi, double *yi, double *a, int *pi) const
{
  double l = getLength();

  //---

  *xi = 0;
  *yi = 0;
  *pi = 0;

  auto ppart = parts_.begin();

  CPoint2D p(0, 0);

  for ( ; ppart != parts_.end(); ++ppart) {
    if ((*ppart)->getType() == CSVGPathPartType::MOVE_TO ||
        (*ppart)->getType() == CSVGPathPartType::RMOVE_TO) {
      p = (*ppart)->getEndPoint(p);
      break;
    }

    ++(*pi);
  }

  CSVGPathPart *part = 0;

  CPoint2D p1 = p, p2 = p;
  double   l1 = 0, l2 = 0;
  double   s1 = 0, s2 = 0;

  for ( ; ppart != parts_.end(); ++ppart) {
    part = *ppart;

    p1 = p2  ; p2 = part->getEndPoint(p1);
    l1 = l2  ; l2 = l1 + part->getLength(p1);
    s1 = l1/l; s2 = l2/l;

    if (s >= s1 && s <= s2 && s2 > s1)
      break;

    ++(*pi);
  }

  if (! part)
    return false;

  CPoint2D ip;

  double ss = CSVGUtil::map(s, s1, s2, 0, 1);

  part->interp(ss, p1, p2, ip, *a);

  *xi = ip.x;
  *yi = ip.y;

  return true;
}

bool
CSVGPathPartList::
getBBox(CSVGBuffer *buffer, CBBox2D &bbox) const
{
  buffer->pathInit();

  for (const auto &p : parts_)
    p->draw(buffer);

  buffer->pathBBox(bbox);

  return bbox.isSet();
}

void
CSVGPathPartList::
moveBy(const CVector2D &d)
{
  for (const auto &p : parts_)
    p->moveBy(d);
}

void
CSVGPathPartList::
print(std::ostream &os) const
{
  int i = 0;

  for (const auto &p : parts_) {
    if (i > 0) os << " ";

    p->print(os);

    ++i;
  }
}

//------

CSVGPathMoveTo::
CSVGPathMoveTo(CSVG &svg, double x, double y) :
 CSVGPathPart(svg, CSVGPathPartType::MOVE_TO), point_(x, y)
{
}

void
CSVGPathMoveTo::
moveBy(const CVector2D &d)
{
  point_ += d;
}

void
CSVGPathMoveTo::
draw(CSVGBuffer *buffer)
{
  buffer->pathMoveTo(point_.x, point_.y);
}

void
CSVGPathMoveTo::
print(std::ostream &os) const
{
  os << "M " << point_.x << " " << point_.y;
}

//------

CSVGPathRMoveTo::
CSVGPathRMoveTo(CSVG &svg, double x, double y) :
 CSVGPathPart(svg, CSVGPathPartType::RMOVE_TO), point_(x, y)
{
}

void
CSVGPathRMoveTo::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathRMoveTo::
draw(CSVGBuffer *buffer)
{
  buffer->pathRMoveTo(point_.x, point_.y);
}

void
CSVGPathRMoveTo::
print(std::ostream &os) const
{
  os << "m " << point_.x << " " << point_.y;
}

//------

CSVGPathLineTo::
CSVGPathLineTo(CSVG &svg, double x, double y) :
 CSVGPathPart(svg, CSVGPathPartType::LINE_TO), point_(x, y)
{
}

void
CSVGPathLineTo::
moveBy(const CVector2D &d)
{
  point_ += d;
}

void
CSVGPathLineTo::
draw(CSVGBuffer *buffer)
{
  buffer->pathLineTo(point_.x, point_.y);
}

double
CSVGPathLineTo::
getLength(const CPoint2D &p) const
{
  return point_.distanceTo(p);
}

bool
CSVGPathLineTo::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  double xi = CSVGUtil::map(s, 0, 1, p1.x, point_.x);
  double yi = CSVGUtil::map(s, 0, 1, p1.y, point_.y);

  pi = CPoint2D(xi, yi);
  a  = CMathGen::atan2(point_.x - p1.x, point_.y - p1.y);

  return true;
}

void
CSVGPathLineTo::
print(std::ostream &os) const
{
  os << "L " << point_.x << " " << point_.y;
}

//------

CSVGPathRLineTo::
CSVGPathRLineTo(CSVG &svg, double x, double y) :
 CSVGPathPart(svg, CSVGPathPartType::RLINE_TO), point_(x, y)
{
}

void
CSVGPathRLineTo::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathRLineTo::
draw(CSVGBuffer *buffer)
{
  buffer->pathRLineTo(point_.x, point_.y);
}

double
CSVGPathRLineTo::
getLength(const CPoint2D &) const
{
  return point_.distanceTo(CPoint2D(0, 0));
}

void
CSVGPathRLineTo::
print(std::ostream &os) const
{
  os << "l " << point_.x << " " << point_.y;
}

//------

CSVGPathHLineTo::
CSVGPathHLineTo(CSVG &svg, double x) :
 CSVGPathPart(svg, CSVGPathPartType::HLINE_TO), x_(x)
{
}

double
CSVGPathHLineTo::
getLength(const CPoint2D &p) const
{
  return fabs(x_ - p.x);
}

void
CSVGPathHLineTo::
moveBy(const CVector2D &d)
{
  // TODO: change to line to
  x_ += d.getX();

  assert(false);
}

void
CSVGPathHLineTo::
draw(CSVGBuffer *buffer)
{
  double x1, y1;

  buffer->pathGetCurrentPoint(&x1, &y1);

  buffer->pathLineTo(x_, y1);
}

void
CSVGPathHLineTo::
print(std::ostream &os) const
{
  os << "H " << x_;
}

//------

CSVGPathRHLineTo::
CSVGPathRHLineTo(CSVG &svg, double dx) :
 CSVGPathPart(svg, CSVGPathPartType::RHLINE_TO), dx_(dx)
{
}

void
CSVGPathRHLineTo::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathRHLineTo::
draw(CSVGBuffer *buffer)
{
  double x1, y1;

  buffer->pathGetCurrentPoint(&x1, &y1);

  buffer->pathLineTo(x1 + dx_, y1);
}

void
CSVGPathRHLineTo::
print(std::ostream &os) const
{
  os << "h " << dx_;
}

//------

CSVGPathVLineTo::
CSVGPathVLineTo(CSVG &svg, double y) :
 CSVGPathPart(svg, CSVGPathPartType::VLINE_TO), y_(y)
{
}

double
CSVGPathVLineTo::
getLength(const CPoint2D &p) const
{
  return fabs(y_ - p.y);
}

void
CSVGPathVLineTo::
moveBy(const CVector2D &d)
{
  // TODO: change to line to
  y_ += d.getX();

  assert(false);
}

void
CSVGPathVLineTo::
draw(CSVGBuffer *buffer)
{
  double x1, y1;

  buffer->pathGetCurrentPoint(&x1, &y1);

  buffer->pathLineTo(x1, y_);
}

void
CSVGPathVLineTo::
print(std::ostream &os) const
{
  os << "V " << y_;
}

//------

CSVGPathRVLineTo::
CSVGPathRVLineTo(CSVG &svg, double dy) :
 CSVGPathPart(svg, CSVGPathPartType::RVLINE_TO), dy_(dy)
{
}

void
CSVGPathRVLineTo::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathRVLineTo::
draw(CSVGBuffer *buffer)
{
  double x1, y1;

  buffer->pathGetCurrentPoint(&x1, &y1);

  buffer->pathLineTo(x1, y1 + dy_);
}

void
CSVGPathRVLineTo::
print(std::ostream &os) const
{
  os << "v " << dy_;
}

//------

CSVGPathArcTo::
CSVGPathArcTo(CSVG &svg, double rx, double ry, double xa, int fa, int fs, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::ARC_TO), rx_(rx), ry_(ry), xa_(xa),
 fa_(fa), fs_(fs), point2_(x2, y2)
{
}

void
CSVGPathArcTo::
moveBy(const CVector2D &d)
{
  point2_ += d;

  invalidate();
}

void
CSVGPathArcTo::
draw(CSVGBuffer *buffer)
{
  double x, y;

  buffer->pathGetCurrentPoint(&x, &y);

  bool   unit_circle = true;
  double cx, cy, rx, ry, theta, delta;

  CSVGUtil::convertArcCoords(x, y, point2_.x, point2_.y, xa_, rx_, ry_, fa_, fs_, unit_circle,
                             &cx, &cy, &rx, &ry, &theta, &delta);

  if (unit_circle) {
    int n_segs = int(ceil(fabs(delta/(90.0 + 0.001))));

    for (int i = 0; i < n_segs; ++i)
      buffer->pathArcSegment(cx, cy, theta + (i + 0)*delta/n_segs,
                             theta + (i + 1)*delta/n_segs, rx, ry, xa_);
  }
  else
    buffer->pathArcTo(cx, cy, rx, ry, CMathGen::DegToRad(theta),
                      CMathGen::DegToRad(theta + delta));
}

double
CSVGPathArcTo::
getLength(const CPoint2D &p) const
{
  if (length_.isValid())
    return length_.getValue();

  //---

  bool   unit_circle = false;
  double cx, cy, rx, ry, theta, delta;

  CSVGUtil::convertArcCoords(p.x, p.y, point2_.x, point2_.y, xa_, rx_, ry_, fa_, fs_,
                             unit_circle, &cx, &cy, &rx, &ry, &theta, &delta);

  CArcToBezier::BezierList beziers;

  CMathGeom2D::ArcToBeziers(cx, cy, rx, ry, CMathGen::DegToRad(theta),
                            CMathGen::DegToRad(theta + delta), beziers);

  double l = 0;

  for (const auto &b : beziers)
    l += b.arcLength();

  //---

  length_ = l;

  return l;
}

bool
CSVGPathArcTo::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  double l = getLength(p1);

  //---

  bool   unit_circle = false;
  double cx, cy, rx, ry, theta, delta;

  CSVGUtil::convertArcCoords(p1.x, p1.y, point2_.x, point2_.y, xa_, rx_, ry_, fa_, fs_,
                             unit_circle, &cx, &cy, &rx, &ry, &theta, &delta);

  CArcToBezier::BezierList beziers;

  CMathGeom2D::ArcToBeziers(cx, cy, rx, ry, CMathGen::DegToRad(theta),
                            CMathGen::DegToRad(theta + delta), beziers);

  double t = CSVGUtil::map(s, 0, 1, 0, l);

  double l1 = 0, l2 = 0;

  for (const auto &b : beziers) {
    l1 = l2;
    l2 = l1 + b.arcLength();

    double xi, yi;

    if (t >= l1 && t <= l2) {
      double t1 = CSVGUtil::map(t, l1, l2, 0, 1);

      b.calc(t1, &xi, &yi);

      pi = CPoint2D(xi, yi);
      a  = b.gradient(t1);

      return true;
    }
  }

  return false;
}

void
CSVGPathArcTo::
print(std::ostream &os) const
{
  os << "A " << rx_ << " " << ry_ << " " << xa_ << " " <<
        fa_ << " " << fs_ << " " << point2_.x << " " << point2_.y;
}

//------

CSVGPathRArcTo::
CSVGPathRArcTo(CSVG &svg, double rx, double ry, double xa, int fa, int fs, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::RARC_TO), rx_(rx), ry_(ry), xa_(xa),
 fa_(fa), fs_(fs), point2_(x2, y2)
{
}

void
CSVGPathRArcTo::
moveBy(const CVector2D &)
{
}

void
CSVGPathRArcTo::
draw(CSVGBuffer *buffer)
{
  double x, y;

  buffer->pathGetCurrentPoint(&x, &y);

  bool   unit_circle = true;
  double cx, cy, rx, ry, theta, delta;

  CSVGUtil::convertArcCoords(x, y, x + point2_.x, y + point2_.y, xa_, rx_, ry_, fa_, fs_,
                             unit_circle, &cx, &cy, &rx, &ry, &theta, &delta);

  if (unit_circle) {
    int n_segs = int(ceil(fabs(delta/(90.0 + 0.001))));

    for (int i = 0; i < n_segs; ++i)
      buffer->pathArcSegment(cx, cy, theta + (i + 0)*delta/n_segs,
                             theta + (i + 1)*delta/n_segs, rx, ry, xa_);
  }
  else
    buffer->pathArcTo(cx, cy, rx, ry, CMathGen::DegToRad(theta),
                      CMathGen::DegToRad(theta + delta));
}

void
CSVGPathRArcTo::
print(std::ostream &os) const
{
  os << "a " << rx_ << " " << ry_ << " " << xa_ << " " <<
        fa_ << " " << fs_ << " " << point2_.x << " " << point2_.y;
}

//------

CSVGPathBezier2To::
CSVGPathBezier2To(CSVG &svg, double x1, double y1, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::BEZIER2_TO), point1_(x1, y1), point2_(x2, y2)
{
}

void
CSVGPathBezier2To::
moveBy(const CVector2D &d)
{
  point1_ += d;
  point2_ += d;
}

void
CSVGPathBezier2To::
draw(CSVGBuffer *buffer)
{
  buffer->pathBezier2To(point1_.x, point1_.y, point2_.x, point2_.y);

  buffer->pathSetLastControlPoint1(point1_);
  buffer->pathSetLastControlPoint2(point2_);
}

bool
CSVGPathBezier2To::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  C2Bezier2D bezier(p1.x, p1.y, point1_.x, point1_.y, point2_.x, point2_.y);

  double xi, yi;

  bezier.calc(s, &xi, &yi);

  pi = CPoint2D(xi, yi);
  a  = bezier.gradient(s);

  return true;
}

void
CSVGPathBezier2To::
print(std::ostream &os) const
{
  os << "Q " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y;
}

//------

CSVGPathMBezier2To::
CSVGPathMBezier2To(CSVG &svg, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::MBEZIER2_TO), point2_(x2, y2)
{
}

void
CSVGPathMBezier2To::
moveBy(const CVector2D &d)
{
  point2_ += d;
}

void
CSVGPathMBezier2To::
draw(CSVGBuffer *buffer)
{
  buffer->pathGetLastMControlPoint(point1_);

  buffer->pathBezier2To(point1_.x, point1_.y, point2_.x, point2_.y);

  buffer->pathSetLastControlPoint1(point1_);
  buffer->pathSetLastControlPoint2(point2_);
}

bool
CSVGPathMBezier2To::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  C2Bezier2D bezier(p1.x, p1.y, point1_.x, point1_.y, point2_.x, point2_.y);

  double xi, yi;

  bezier.calc(s, &xi, &yi);

  pi = CPoint2D(xi, yi);
  a  = bezier.gradient(s);

  return true;
}

void
CSVGPathMBezier2To::
print(std::ostream &os) const
{
  os << "T " << point2_.x << " " << point2_.y;
}

//------

CSVGPathRBezier2To::
CSVGPathRBezier2To(CSVG &svg, double x1, double y1, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::RBEZIER2_TO), point1_(x1, y1), point2_(x2, y2)
{
}

void
CSVGPathRBezier2To::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathRBezier2To::
draw(CSVGBuffer *buffer)
{
  CPoint2D cp;

  buffer->pathGetCurrentPoint(cp);

  buffer->pathRBezier2To(point1_.x, point1_.y, point2_.x, point2_.y);

  buffer->pathSetLastControlPoint1(cp + point1_);
  buffer->pathSetLastControlPoint2(cp + point2_);
}

bool
CSVGPathRBezier2To::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  CPoint2D p11 = p1 + point1_;
  CPoint2D p12 = p1 + point2_;

  C2Bezier2D bezier(p1.x, p1.y, p11.x, p11.y, p12.x, p12.y);

  double xi, yi;

  bezier.calc(s, &xi, &yi);

  pi = CPoint2D(xi, yi);
  a  = bezier.gradient(s);

  return true;
}

void
CSVGPathRBezier2To::
print(std::ostream &os) const
{
  os << "q " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y;
}

//------

CSVGPathMRBezier2To::
CSVGPathMRBezier2To(CSVG &svg, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::MRBEZIER2_TO), point2_(x2, y2)
{
}

void
CSVGPathMRBezier2To::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathMRBezier2To::
draw(CSVGBuffer *buffer)
{
  CPoint2D cp;

  buffer->pathGetCurrentPoint(cp);

  buffer->pathGetLastMRControlPoint(point1_);

  buffer->pathRBezier2To(point1_.x, point1_.y, point2_.x, point2_.y);

  buffer->pathSetLastControlPoint1(cp + point1_);
  buffer->pathSetLastControlPoint2(cp + point2_);
}

bool
CSVGPathMRBezier2To::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  CPoint2D p11 = p1 + point1_;
  CPoint2D p12 = p1 + point2_;

  C2Bezier2D bezier(p1.x, p1.y, p11.x, p11.y, p12.x, p12.y);

  double xi, yi;

  bezier.calc(s, &xi, &yi);

  pi = CPoint2D(xi, yi);
  a  = bezier.gradient(s);

  return true;
}

void
CSVGPathMRBezier2To::
print(std::ostream &os) const
{
  os << "t " << point2_.x << " " << point2_.y;
}

//------

CSVGPathBezier3To::
CSVGPathBezier3To(CSVG &svg, double x1, double y1, double x2, double y2, double x3, double y3) :
 CSVGPathPart(svg, CSVGPathPartType::BEZIER3_TO),
 point1_(x1, y1), point2_(x2, y2), point3_(x3, y3)
{
}

void
CSVGPathBezier3To::
moveBy(const CVector2D &d)
{
  point1_ += d;
  point2_ += d;
  point3_ += d;
}

void
CSVGPathBezier3To::
draw(CSVGBuffer *buffer)
{
  buffer->pathBezier3To(point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  buffer->pathSetLastControlPoint1(point2_);
  buffer->pathSetLastControlPoint2(point3_);
}

double
CSVGPathBezier3To::
getLength(const CPoint2D &p) const
{
  if (length_.isValid())
    return length_.getValue();

  //---

  C3Bezier2D bezier(p.x, p.y, point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  double l = bezier.arcLength();

  //---

  length_ = l;

  return l;
}

bool
CSVGPathBezier3To::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  C3Bezier2D bezier(p1.x, p1.y, point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  double xi, yi;

  bezier.calc(s, &xi, &yi);

  pi = CPoint2D(xi, yi);
  a  = bezier.gradient(s);

  return yi;
}

void
CSVGPathBezier3To::
print(std::ostream &os) const
{
  os << "C " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y << " " <<
                point3_.x << " " << point3_.y;
}

//------

CSVGPathMBezier3To::
CSVGPathMBezier3To(CSVG &svg, double x2, double y2, double x3, double y3) :
 CSVGPathPart(svg, CSVGPathPartType::MBEZIER3_TO), point2_(x2, y2), point3_(x3, y3)
{
}

void
CSVGPathMBezier3To::
moveBy(const CVector2D &d)
{
  point2_ += d;
  point3_ += d;
}

void
CSVGPathMBezier3To::
draw(CSVGBuffer *buffer)
{
  buffer->pathGetLastMControlPoint(point1_);

  buffer->pathBezier3To(point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  buffer->pathSetLastControlPoint1(point2_);
  buffer->pathSetLastControlPoint2(point3_);
}

double
CSVGPathMBezier3To::
getLength(const CPoint2D &p) const
{
  if (length_.isValid())
    return length_.getValue();

  //---

  C3Bezier2D bezier(p.x, p.y, point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  double l = bezier.arcLength();

  //---

  length_ = l;

  return l;
}

bool
CSVGPathMBezier3To::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  C3Bezier2D bezier(p1.x, p1.y, point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  double xi, yi;

  bezier.calc(s, &xi, &yi);

  pi = CPoint2D(xi, yi);
  a  = bezier.gradient(s);

  return yi;
}

void
CSVGPathMBezier3To::
print(std::ostream &os) const
{
  os << "S " << point2_.x << " " << point2_.y << " " << point3_.x << " " << point3_.y;
}

//------

CSVGPathRBezier3To::
CSVGPathRBezier3To(CSVG &svg, double x1, double y1, double x2, double y2, double x3, double y3) :
 CSVGPathPart(svg, CSVGPathPartType::RBEZIER3_TO),
 point1_(x1, y1), point2_(x2, y2), point3_(x3, y3)
{
}

void
CSVGPathRBezier3To::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathRBezier3To::
draw(CSVGBuffer *buffer)
{
  CPoint2D cp;

  buffer->pathGetCurrentPoint(cp);

  buffer->pathRBezier3To(point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  buffer->pathSetLastControlPoint1(cp + point2_);
  buffer->pathSetLastControlPoint2(cp + point3_);
}

double
CSVGPathRBezier3To::
getLength(const CPoint2D &) const
{
  if (length_.isValid())
    return length_.getValue();

  //---

  C3Bezier2D bezier(0, 0, point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  double l = bezier.arcLength();

  //---

  length_ = l;

  return l;
}

bool
CSVGPathRBezier3To::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  CPoint2D p11 = p1 + point1_;
  CPoint2D p12 = p1 + point2_;
  CPoint2D p13 = p1 + point3_;

  C3Bezier2D bezier(p1.x, p1.y, p11.x, p11.y, p12.x, p12.y, p13.x, p13.y);

  double xi, yi;

  bezier.calc(s, &xi, &yi);

  pi = CPoint2D(xi, yi);
  a  = bezier.gradient(s);

  return yi;
}

void
CSVGPathRBezier3To::
print(std::ostream &os) const
{
  os << "c " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y << " " <<
                point3_.x << " " << point3_.y;
}

//------

CSVGPathMRBezier3To::
CSVGPathMRBezier3To(CSVG &svg, double x2, double y2, double x3, double y3) :
 CSVGPathPart(svg, CSVGPathPartType::MRBEZIER3_TO), point2_(x2, y2), point3_(x3, y3)
{
}

void
CSVGPathMRBezier3To::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathMRBezier3To::
draw(CSVGBuffer *buffer)
{
  CPoint2D cp;

  buffer->pathGetCurrentPoint(cp);

  buffer->pathGetLastMRControlPoint(point1_);

  buffer->pathRBezier3To(point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  buffer->pathSetLastControlPoint1(cp + point2_);
  buffer->pathSetLastControlPoint2(cp + point3_);
}

double
CSVGPathMRBezier3To::
getLength(const CPoint2D &) const
{
  if (length_.isValid())
    return length_.getValue();

  //---

  C3Bezier2D bezier(0, 0, point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  double l = bezier.arcLength();

  //---

  length_ = l;

  return l;
}

bool
CSVGPathMRBezier3To::
interp(double s, const CPoint2D &p1, const CPoint2D & /*p2*/, CPoint2D &pi, double &a) const
{
  CPoint2D p11 = p1 + point1_;
  CPoint2D p12 = p1 + point2_;
  CPoint2D p13 = p1 + point3_;

  C3Bezier2D bezier(p1.x, p1.y, p11.x, p11.y, p12.x, p12.y, p13.x, p13.y);

  double xi, yi;

  bezier.calc(s, &xi, &yi);

  pi = CPoint2D(xi, yi);
  a  = bezier.gradient(s);

  return yi;
}

void
CSVGPathMRBezier3To::
print(std::ostream &os) const
{
  os << "s " << point2_.x << " " << point2_.y << " " << point3_.x << " " << point3_.y;
}

//------

CSVGPathClosePath::
CSVGPathClosePath(CSVG &svg, bool relative) :
 CSVGPathPart(svg, CSVGPathPartType::CLOSE_PATH), relative_(relative)
{
}

void
CSVGPathClosePath::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathClosePath::
draw(CSVGBuffer *buffer)
{
  buffer->pathClose();
}

void
CSVGPathClosePath::
print(std::ostream &os) const
{
  if (relative_)
    os << "z";
  else
    os << "Z";
}
