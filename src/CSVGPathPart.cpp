#include <CSVGPathPart.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>
#include <CStrParse.h>
#include <C2Bezier2D.h>
#include <C3Bezier2D.h>

double
CSVGPathPart::
interp(double xi, const CPoint2D &p1, const CPoint2D &p2, double &a) const
{
std::cerr << "CSVGPathPart::interp" << std::endl;
  double yi = CSVGUtil::map(xi, p1.x, p2.x, p1.y, p2.y);

  a = CMathGen::atan2(p2.x - p1.x, p2.y - p1.y);

  return yi;
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
draw()
{
  svg_.pathMoveTo(point_.x, point_.y);
}

void
CSVGPathMoveTo::
print(std::ostream &os) const
{
  os << "M " << point_.x << " " << point_.y;
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
draw()
{
  svg_.pathLineTo(point_.x, point_.y);
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
draw()
{
  svg_.pathRLineTo(point_.x, point_.y);
}

void
CSVGPathRLineTo::
print(std::ostream &os) const
{
  os << "l " << point_.x << " " << point_.y;
}

//------

CSVGPathHLineTo::
CSVGPathHLineTo(CSVG &svg, double d) :
 CSVGPathPart(svg, CSVGPathPartType::HLINE_TO), d_(d)
{
}

void
CSVGPathHLineTo::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathHLineTo::
draw()
{
  double x1, y1;

  svg_.pathGetCurrentPoint(&x1, &y1);

  svg_.pathLineTo(x1 + d_, y1);
}

void
CSVGPathHLineTo::
print(std::ostream &os) const
{
  os << "H " << d_;
}

//------

CSVGPathVLineTo::
CSVGPathVLineTo(CSVG &svg, double d) :
 CSVGPathPart(svg, CSVGPathPartType::VLINE_TO), d_(d)
{
}

void
CSVGPathVLineTo::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathVLineTo::
draw()
{
  double x1, y1;

  svg_.pathGetCurrentPoint(&x1, &y1);

  svg_.pathLineTo(x1, y1 + d_);
}

void
CSVGPathVLineTo::
print(std::ostream &os) const
{
  os << "V " << d_;
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
}

void
CSVGPathArcTo::
draw()
{
  double x, y, cx, cy, rx, ry, theta, delta;

  svg_.pathGetCurrentPoint(&x, &y);

  bool unit_circle = true;

  CSVGUtil::convertArcCoords(x, y, point2_.x, point2_.y, xa_, rx_, ry_, fa_, fs_, unit_circle,
                             &cx, &cy, &rx, &ry, &theta, &delta);

  if (unit_circle) {
    int n_segs = int(ceil(fabs(delta/(90.0 + 0.001))));

    for (int i = 0; i < n_segs; ++i)
      svg_.pathArcSegment(cx, cy, theta + (i + 0)*delta/n_segs,
                          theta + (i + 1)*delta/n_segs, rx, ry, xa_);
  }
  else
    svg_.pathArcTo(cx, cy, rx, ry, CMathGen::DegToRad(theta),
                   CMathGen::DegToRad(theta + delta));
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
draw()
{
  double x, y, cx, cy, rx, ry, theta, delta;

  svg_.pathGetCurrentPoint(&x, &y);

  bool unit_circle = true;

  CSVGUtil::convertArcCoords(x, y, x + point2_.x, y + point2_.y, xa_, rx_, ry_, fa_, fs_,
                             unit_circle, &cx, &cy, &rx, &ry, &theta, &delta);

  if (unit_circle) {
    int n_segs = int(ceil(fabs(delta/(90.0 + 0.001))));

    for (int i = 0; i < n_segs; ++i)
      svg_.pathArcSegment(cx, cy, theta + (i + 0)*delta/n_segs,
                          theta + (i + 1)*delta/n_segs, rx, ry, xa_);
  }
  else
    svg_.pathArcTo(cx, cy, rx, ry, CMathGen::DegToRad(theta),
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
draw()
{
  svg_.pathBezier2To(point1_.x, point1_.y, point2_.x, point2_.y);
}

double
CSVGPathBezier2To::
interp(double xi, const CPoint2D &p1, const CPoint2D &p2, double &a) const
{
  C2Bezier2D bezier(p1.x, p1.y, point1_.x, point1_.y, point2_.x, point2_.y);

  double t = CSVGUtil::map(xi, p1.x, p2.x, 0, 1);

  double yi;

  bezier.calc(t, &xi, &yi);

  a = bezier.gradient(t);

  return yi;
}

void
CSVGPathBezier2To::
print(std::ostream &os) const
{
  os << "Q " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y;
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
draw()
{
  svg_.pathRBezier2To(point1_.x, point1_.y, point2_.x, point2_.y);
}

double
CSVGPathRBezier2To::
interp(double xi, const CPoint2D &p1, const CPoint2D &p2, double &a) const
{
  CPoint2D p11 = p1 + point1_;
  CPoint2D p12 = p1 + point2_;

  C2Bezier2D bezier(p1.x, p1.y, p11.x, p11.y, p12.x, p12.y);

  double t = CSVGUtil::map(xi, p1.x, p2.x, 0, 1);

  double yi;

  bezier.calc(t, &xi, &yi);

  a = bezier.gradient(t);

  return yi;
}

void
CSVGPathRBezier2To::
print(std::ostream &os) const
{
  os << "q " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y;
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
draw()
{
  svg_.pathBezier3To(point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);
}

double
CSVGPathBezier3To::
interp(double xi, const CPoint2D &p1, const CPoint2D &p2, double &a) const
{
  C3Bezier2D bezier(p1.x, p1.y, point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);

  double t = CSVGUtil::map(xi, p1.x, p2.x, 0, 1);

  double yi;

  bezier.calc(t, &xi, &yi);

  a = bezier.gradient(t);

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
draw()
{
  svg_.pathRBezier3To(point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);
}

double
CSVGPathRBezier3To::
interp(double xi, const CPoint2D &p1, const CPoint2D &p2, double &a) const
{
  CPoint2D p11 = p1 + point1_;
  CPoint2D p12 = p1 + point2_;
  CPoint2D p13 = p1 + point3_;

  C3Bezier2D bezier(p1.x, p1.y, p11.x, p11.y, p12.x, p12.y, p13.x, p13.y);

  double t = CSVGUtil::map(xi, p1.x, p2.x, 0, 1);

  double yi;

  bezier.calc(t, &xi, &yi);

  a = bezier.gradient(t);

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

CSVGPathClosePath::
CSVGPathClosePath(CSVG &svg) :
 CSVGPathPart(svg, CSVGPathPartType::CLOSE_PATH)
{
}

void
CSVGPathClosePath::
moveBy(const CVector2D & /*d*/)
{
}

void
CSVGPathClosePath::
draw()
{
  svg_.pathClose();
}

void
CSVGPathClosePath::
print(std::ostream &os) const
{
  os << "z";
}
