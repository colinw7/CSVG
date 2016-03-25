#include <CSVGPath.h>
#include <CSVGPathPart.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>
#include <CStrParse.h>

CSVGPath::
CSVGPath(CSVG &svg) :
 CSVGObject(svg),
 parts_    ()
{
  //fill_.setDefColor(CRGBA(0,0,0));
}

CSVGPath::
CSVGPath(const CSVGPath &path) :
 CSVGObject(path),
 parts_    (path.parts_)
{
}

CSVGPath *
CSVGPath::
dup() const
{
  return new CSVGPath(*this);
}

bool
CSVGPath::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if (svg_.stringOption(opt_name, opt_value, "d", str)) {
    if (! svg_.pathStringToParts(str, parts_))
      return false;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.drawParts(parts_, &marker_);
}

void
CSVGPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<path";

    std::string id = getId();

    if (id != "")
      os << " id=\"" << id << "\"";

    os << " d=\"";

    svg_.printParts(os, parts_);

    os << "\"";

    printStyle(os);

    os << "/>" << std::endl;
  }
  else {
    os << "path (";

    svg_.printParts(os, parts_);

    os << ")";
  }
}

bool
CSVGPath::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isSet())
    return svg_.getPartsBBox(parts_, bbox);
  else {
    bbox = viewBox_;

    return true;
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGPath &path)
{
  path.print(os, false);

  return os;
}

//-----------

CSVGPathMoveTo::
CSVGPathMoveTo(CSVG &svg, double x, double y) :
 CSVGPathPart(svg, CSVGPathPartType::MOVE_TO), point_(x, y)
{
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

CSVGPathLineTo::
CSVGPathLineTo(CSVG &svg, double x, double y) :
 CSVGPathPart(svg, CSVGPathPartType::LINE_TO), point_(x, y)
{
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

CSVGPathRLineTo::
CSVGPathRLineTo(CSVG &svg, double x, double y) :
 CSVGPathPart(svg, CSVGPathPartType::RLINE_TO), point_(x, y)
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

CSVGPathHLineTo::
CSVGPathHLineTo(CSVG &svg, double d) :
 CSVGPathPart(svg, CSVGPathPartType::HLINE_TO), d_(d)
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

CSVGPathVLineTo::
CSVGPathVLineTo(CSVG &svg, double d) :
 CSVGPathPart(svg, CSVGPathPartType::VLINE_TO), d_(d)
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

CSVGPathArcTo::
CSVGPathArcTo(CSVG &svg, double rx, double ry, double xa, int fa, int fs, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::ARC_TO), rx_(rx), ry_(ry), xa_(xa),
 fa_(fa), fs_(fs), point2_(x2, y2)
{
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

CSVGPathRArcTo::
CSVGPathRArcTo(CSVG &svg, double rx, double ry, double xa, int fa, int fs, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::RARC_TO), rx_(rx), ry_(ry), xa_(xa),
 fa_(fa), fs_(fs), point2_(x2, y2)
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

CSVGPathBezier2To::
CSVGPathBezier2To(CSVG &svg, double x1, double y1, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::BEZIER2_TO), point1_(x1, y1), point2_(x2, y2)
{
}

void
CSVGPathBezier2To::
draw()
{
  svg_.pathBezier2To(point1_.x, point1_.y, point2_.x, point2_.y);
}

void
CSVGPathBezier2To::
print(std::ostream &os) const
{
  os << "Q " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y;
}

CSVGPathRBezier2To::
CSVGPathRBezier2To(CSVG &svg, double x1, double y1, double x2, double y2) :
 CSVGPathPart(svg, CSVGPathPartType::RBEZIER2_TO), point1_(x1, y1), point2_(x2, y2)
{
}

void
CSVGPathRBezier2To::
draw()
{
  svg_.pathRBezier2To(point1_.x, point1_.y, point2_.x, point2_.y);
}

void
CSVGPathRBezier2To::
print(std::ostream &os) const
{
  os << "q " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y;
}

CSVGPathBezier3To::
CSVGPathBezier3To(CSVG &svg, double x1, double y1, double x2, double y2, double x3, double y3) :
 CSVGPathPart(svg, CSVGPathPartType::BEZIER3_TO),
 point1_(x1, y1), point2_(x2, y2), point3_(x3, y3)
{
}

void
CSVGPathBezier3To::
draw()
{
  svg_.pathBezier3To(point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);
}

void
CSVGPathBezier3To::
print(std::ostream &os) const
{
  os << "C " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y << " " <<
                point3_.x << " " << point3_.y;
}

CSVGPathRBezier3To::
CSVGPathRBezier3To(CSVG &svg, double x1, double y1, double x2, double y2, double x3, double y3) :
 CSVGPathPart(svg, CSVGPathPartType::RBEZIER3_TO),
 point1_(x1, y1), point2_(x2, y2), point3_(x3, y3)
{
}

void
CSVGPathRBezier3To::
draw()
{
  svg_.pathRBezier3To(point1_.x, point1_.y, point2_.x, point2_.y, point3_.x, point3_.y);
}

void
CSVGPathRBezier3To::
print(std::ostream &os) const
{
  os << "c " << point1_.x << " " << point1_.y << " " <<
                point2_.x << " " << point2_.y << " " <<
                point3_.x << " " << point3_.y;
}

CSVGPathClosePath::
CSVGPathClosePath(CSVG &svg) :
 CSVGPathPart(svg, CSVGPathPartType::CLOSE_PATH)
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
