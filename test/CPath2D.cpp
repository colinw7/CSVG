#include <CPath2D.h>
#include <CPath2DRenderer.h>
#include <CPath2DFlattener.h>
#include <CFreeTypePath2D.h>
#include <CTriangle2D.h>
#include <CLineList2D.h>
#include <CStrUtil.h>
#include <CFreeType.h>

CPath2D::
CPath2D() :
 renderer_     (NULL),
 flattener_    (NULL),
 parts_        (),
 closed_       (false),
 start1_       (0, 0 ),
 start2_       (0, 0),
 current_      (0, 0),
 current_set_  (false),
 stroke_adjust_(false),
 flat_fill_    (false),
 fill_type_    (FILL_TYPE_WINDING),
 fill_points_  ()
{
  init();
}

CPath2D::
CPath2D(const CPath2D &path) :
 renderer_     (path.renderer_     ),
 flattener_    (path.flattener_    ),
 parts_        (path.parts_        ),
 closed_       (path.closed_       ),
 start1_       (path.start1_       ),
 start2_       (path.start2_       ),
 current_      (path.current_      ),
 current_set_  (path.current_set_  ),
 stroke_adjust_(path.stroke_adjust_),
 flat_fill_    (path.flat_fill_    ),
 fill_type_    (path.fill_type_    ),
 fill_points_  (path.fill_points_  )
{
}

CPath2D::
~CPath2D()
{
}

const CPath2D &
CPath2D::
operator=(const CPath2D &path)
{
  renderer_      = path.renderer_;
  flattener_     = path.flattener_;
  parts_         = path.parts_;
  closed_        = path.closed_;
  start1_        = path.start1_;
  start2_        = path.start2_;
  current_       = path.current_;
  current_set_   = path.current_set_;
  stroke_adjust_ = path.stroke_adjust_;
  flat_fill_     = path.flat_fill_;
  fill_type_     = path.fill_type_;
  fill_points_   = path.fill_points_;

  return *this;
}

CPath2D *
CPath2D::
dup()
{
  CPath2D *path = new CPath2D(*this);

  return path;
}

void
CPath2D::
init()
{
  closed_ = false;

  start1_.zero();
  start2_.zero();

  current_.zero();

  current_set_ = false;

  stroke_adjust_ = false;

  flat_fill_ = true;

  fill_type_ = FILL_TYPE_WINDING;

  parts_.resize(0);

  if (renderer_)
    renderer_->initPolygons();

  fill_points_.clear();
}

void
CPath2D::
ellipse(const CPoint2D &center, double xr, double yr)
{
  double angle1 = 0.0;
  double angle2 = 2.0*M_PI;

  std::vector<C3Bezier2D> beziers;

  if (flattener_)
    flattener_->arcToBeziers(center, xr, yr, angle1, angle2, beziers);
  else
    CMathGeom2D::ArcToBeziers(center.x, center.y, xr, yr, angle1, angle2, beziers);

  uint num_beziers = beziers.size();

  for (uint i = 0; i < num_beziers; ++i)
    bezier(beziers[i]);
}

void
CPath2D::
text(const std::string &str, CFontPtr font)
{
  CPoint2D current;

  getCurrentPoint(current);

  CFreeTypeMgrInst->setFont(font);

  CFreeTypePath2D ft(this);

  CFreeTypeMgrInst->pathString(&ft, str.c_str(), &current.x, &current.y);

  current_ = current;

  current_set_ = true;
}

void
CPath2D::
bezier(C3Bezier2D &bezier)
{
  std::vector<CPoint2D> points;
  std::vector<double>   gradients;

  if (flattener_)
    flattener_->bezierToLines(bezier, points, gradients);
  else
    CMathGeom2D::BezierToLines(bezier, points);

  uint num_points = points.size();

  for (uint i = 0; i < num_points; ++i)
    lineTo(points[i]);
}

void
CPath2D::
bezier(C2Bezier2D &bezier)
{
  std::vector<CPoint2D> points;
  std::vector<double>   gradients;

  if (flattener_)
    flattener_->bezierToLines(bezier, points, gradients);
  else
    CMathGeom2D::BezierToLines(bezier, points);

  uint num_points = points.size();

  for (uint i = 0; i < num_points; ++i)
    lineTo(points[i]);
}

void
CPath2D::
polygon(const std::vector<CPoint2D> &points)
{
  uint num_points = points.size();

  for (uint i = 0; i < num_points; ++i)
    lineTo(points[i]);
}

void
CPath2D::
roundedRectangle(const CPoint2D &point1, const CPoint2D &point2, double rx, double ry)
{
  init();

  moveTo(CPoint2D(point1.x + rx, point1.y));

  lineTo(CPoint2D(point2.x - rx, point1.y));

  arc(CPoint2D(point2.x - rx, point1.y + ry), rx, ry, PI3_DIV_2, PI2);

  lineTo(CPoint2D(point2.x, point1.y + ry));

  arc(CPoint2D(point2.x - rx, point2.y - ry), rx, ry, 0        , PI_DIV_2);

  lineTo(CPoint2D(point2.x - rx, point2.y));

  arc(CPoint2D(point1.x + rx, point2.y - ry), rx, ry, PI_DIV_2 , M_PI);

  lineTo(CPoint2D(point1.x, point2.y - ry));

  arc(CPoint2D(point1.x + rx, point1.y + ry), rx, ry, M_PI     , PI3_DIV_2);

  close();
}

void
CPath2D::
rectangle(const CPoint2D &point1, const CPoint2D &point2)
{
  init();

  lineTo(CPoint2D(point1));
  lineTo(CPoint2D(point2.x, point1.y));
  lineTo(CPoint2D(point2));
  lineTo(CPoint2D(point1.x, point2.y));

  close();
}

bool
CPath2D::
moveTo(const CPoint2D &point)
{
  CPoint2D t;

  transformPoint(point, t);

  uint num_parts = parts_.size();

  if (num_parts > 0 && parts_[num_parts - 1].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO)
    parts_[num_parts - 1] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_MOVE_TO, t);
  else {
    parts_.resize(parts_.size() + 1);

    parts_[num_parts] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_MOVE_TO, t);
  }

  closed_      = false;
  start1_      = t;
  start2_      = point;
  current_     = start2_;
  current_set_ = true;

  return true;
}

bool
CPath2D::
rmoveTo(const CPoint2D &point)
{
  CPoint2D current;

  if (getCurrentPoint(current))
    moveTo(current + point);
  else
    moveTo(point);

  return true;
}

bool
CPath2D::
lineTo(const CPoint2D &point)
{
  if (! current_set_)
    return moveTo(point);

  if (closed_)
    moveTo(start2_);

  CPoint2D t;

  transformPoint(point, t);

  uint num_parts = parts_.size();

  parts_.resize(parts_.size() + 1);

  parts_[num_parts] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_LINE_TO, t);

  current_ = point;
  closed_  = false;

  return true;
}

bool
CPath2D::
rlineTo(const CPoint2D &point)
{
  if (! current_set_)
    return rmoveTo(point);

  CPoint2D current;

  if (! getCurrentPoint(current))
    return false;

  lineTo(current + point);

  return true;
}

void
CPath2D::
arc(const CPoint2D &center, double xr, double yr, double angle1, double angle2)
{
  std::vector<C3Bezier2D> beziers;

  if (flattener_)
    flattener_->arcToBeziers(center, xr, yr, angle1, angle2, beziers);
  else
    CMathGeom2D::ArcToBeziers(center.x, center.y, xr, yr, angle1, angle2, beziers);

  uint num_beziers = beziers.size();

  if (num_beziers > 0) {
    double x1, y1, x2, y2, x3, y3, x4, y4;

    beziers[0].getFirstPoint(&x1, &y1);

    if (current_set_)
      lineTo(CPoint2D(x1, y1));
    else
      moveTo(CPoint2D(x1, y1));

    for (uint i = 0; i < num_beziers; i++) {
      beziers[i].getControlPoint1(&x2, &y2);
      beziers[i].getControlPoint2(&x3, &y3);
      beziers[i].getLastPoint    (&x4, &y4);

      bezier3To(CPoint2D(x2, y2), CPoint2D(x3, y3), CPoint2D(x4, y4));
    }
  }
  else {
    if (current_set_)
      lineTo(CPoint2D(center.x + xr*cos(angle1), center.y + yr*sin(angle1)));
    else
      moveTo(CPoint2D(center.x + xr*cos(angle1), center.y + yr*sin(angle1)));

    lineTo(CPoint2D(center.x + xr*cos(angle2), center.y + yr*sin(angle2)));
  }
}

void
CPath2D::
arcN(const CPoint2D &center, double xr, double yr, double angle1, double angle2)
{
  std::vector<C3Bezier2D> beziers;

  if (flattener_)
    flattener_->arcNToBeziers(center, xr, yr, angle1, angle2, beziers);
  else
    CMathGeom2D::ArcNToBeziers(center.x, center.y, xr, yr, angle1, angle2, beziers);

  uint num_beziers = beziers.size();

  if (num_beziers > 0) {
    double x1, y1, x2, y2, x3, y3, x4, y4;

    beziers[0].getFirstPoint(&x1, &y1);

    if (current_set_)
      lineTo(CPoint2D(x1, y1));
    else
      moveTo(CPoint2D(x1, y1));

    for (uint i = 0; i < num_beziers; i++) {
      beziers[i].getControlPoint1(&x2, &y2);
      beziers[i].getControlPoint2(&x3, &y3);
      beziers[i].getLastPoint    (&x4, &y4);

      bezier3To(CPoint2D(x2, y2), CPoint2D(x3, y3), CPoint2D(x4, y4));
    }
  }
  else {
    if (current_set_)
      lineTo(CPoint2D(center.x + xr*cos(angle1), center.y + yr*sin(angle1)));
    else
      moveTo(CPoint2D(center.x + xr*cos(angle1), center.y + yr*sin(angle1)));

    lineTo(CPoint2D(center.x + xr*cos(angle2), center.y + yr*sin(angle2)));
  }
}

bool
CPath2D::
arcTo(const CPoint2D &point1, const CPoint2D &point2, double xr, double yr)
{
  if (! current_set_)
    moveTo(point1);

  CPoint2D point0(current_);

  double x[3] = { point0.x, point1.x, point2.x };
  double y[3] = { point0.y, point1.y, point2.y };

  CLineList2D path(x, y, 3);

  double xc, yc, xt1, yt1, xt2, yt2;

  bool flag = path.arcThrough(xr, yr, &xc, &yc, &xt1, &yt1, &xt2, &yt2);

  if (flag) {
    if (xt1 != point0.x || yt1 != point0.y)
      lineTo(CPoint2D(xt1, yt1));

    CTriangle2D triangle(point0, point1, point2);

    if (triangle.orientation() == 1)
      arc (CPoint2D(xc, yc), xr, yr, CMathGen::atan2(xt1 - xc, yt1 - yc),
           CMathGen::atan2(xt2 - xc, yt2 - yc));
    else
      arcN(CPoint2D(xc, yc), xr, yr, CMathGen::atan2(xt1 - xc, yt1 - yc),
           CMathGen::atan2(xt2 - xc, yt2 - yc));
  }
  else
    lineTo(point1);

  return true;
}

bool
CPath2D::
bezier2To(const CPoint2D &point2, const CPoint2D &point3)
{
  if (! current_set_)
    moveTo(CPoint2D(0, 0));

  CPoint2D t2, t3;

  transformPoint(point2, t2);
  transformPoint(point3, t3);

  uint num_parts = parts_.size();

  parts_.resize(parts_.size() + 2);

  parts_[num_parts++] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER2_TO, t2);
  parts_[num_parts++] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER2_TO, t3);

  closed_  = false;
  current_ = point3;

  return true;
}

bool
CPath2D::
rbezier2To(const CPoint2D &point2, const CPoint2D &point3)
{
  CPoint2D current;

  if (! getCurrentPoint(current))
    return false;

  bezier2To(point2 + current, point3 + current);

  return true;
}

bool
CPath2D::
bezier3To(const CPoint2D &point2, const CPoint2D &point3, const CPoint2D &point4)
{
  if (! current_set_)
    moveTo(CPoint2D(0, 0));

  CPoint2D t2, t3, t4;

  transformPoint(point2, t2);
  transformPoint(point3, t3);
  transformPoint(point4, t4);

  uint num_parts = parts_.size();

  parts_.resize(parts_.size() + 3);

  parts_[num_parts++] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER3_TO, t2);
  parts_[num_parts++] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER3_TO, t3);
  parts_[num_parts++] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER3_TO, t4);

  closed_  = false;
  current_ = point4;

  return true;
}

bool
CPath2D::
rbezier3To(const CPoint2D &point2, const CPoint2D &point3, const CPoint2D &point4)
{
  CPoint2D current;

  if (! getCurrentPoint(current))
    return false;

  bezier3To(point2 + current, point3 + current, point4 + current);

  return true;
}

bool
CPath2D::
getCurrentPoint(CPoint2D &point)
{
  CPoint2D point1;

  if (! getCurrentPoint1(point1))
    return false;

  CPoint2D p;

  untransformPoint(point1, p);

  point = p;

  return true;
}

bool
CPath2D::
getCurrentPoint1(CPoint2D &point)
{
  if (current_set_) {
    uint num_parts = parts_.size();

    if (num_parts > 0)
      point = parts_[num_parts - 1].point;
    else
      point = current_;

    return true;
  }
  else {
    point = CPoint2D(0, 0);

    return false;
  }
}

void
CPath2D::
showChar(int c, CMatrix2D *matrix, double *x, double *y)
{
  current_ = CPoint2D(*x, *y);

  //------

  double data[6];

  matrix->getValues(&data[0], &data[2], &data[1], &data[3], &data[4], &data[5]);

  //------

  if (renderer_)
    renderer_->setTextSize(data[0], data[3]);

  std::string str = " ";

  str[0] = c;

  if (renderer_)
    renderer_->drawText(current_, str);

  double text_x, text_y;

  if (renderer_)
    renderer_->getTextLimits(str, &text_x, &text_y);

  *x += text_x;
  *y += text_y;
}

void
CPath2D::
close()
{
  if (! current_set_ || closed_) {
    closed_ = true;

    return;
  }

  uint num_parts = parts_.size();

  parts_.resize(num_parts + 1);

  parts_[num_parts] = CPath2DPart(CPATH_2D_PATH_PART_TYPE_CLOSE_PATH, start1_);

  current_ = start2_;

  closed_ = true;
}

void
CPath2D::
stroke()
{
  stroke(renderer_);
}

void
CPath2D::
stroke(CPath2DRenderer *renderer)
{
  if (! renderer) return;

  CPath2DRendererStroker stroker(renderer, flattener_);

  renderer->setTransform(false);

  stroke(stroker);

  renderer->setTransform(true);
}

void
CPath2D::
stroke(CPath2DStroker &stroker)
{
  double x1 = 0.0;
  double y1 = 0.0;
  double x2 = 0.0;
  double y2 = 0.0;

  //------

  bool closed = false;

  int start_i = 0;

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; i++) {
    if      (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO) {
      if (getStrokeAdjust())
        stroker.adjustPoint(parts_[i].point);

      if (i - start_i > 1 && ! closed) {
        if (parts_[start_i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
            parts_[start_i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO)
          stroker.capLine(CPoint2D(x2, y2), CPoint2D(x1, y1));
        else
          stroker.capLine(CPoint2D(x2, y2), parts_[start_i].point);
      }

      //------

      closed = false;

      uint j = i + 1;

      while (j < num_parts) {
        if (parts_[j].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH) {
          closed = true;
          break;
        }

        j++;
      }

      //------

      if (! closed) {
        if (j - i > 1)
          stroker.capLine(parts_[i].point, parts_[i + 1].point);
        else
          stroker.capLine(parts_[i].point, parts_[i].point);
      }

      //------

      start_i = i;

      x2 = parts_[i].point.x;
      y2 = parts_[i].point.y;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
      if (getStrokeAdjust())
        stroker.adjustPoint(parts_[i].point);

      if (i > 0 && (parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_LINE_TO ||
                    parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
                    parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO))
        stroker.joinLines(CPoint2D(x1, y1), CPoint2D(x2, y2), parts_[i].point);

      stroker.drawLine(CPoint2D(x2, y2), CPoint2D(parts_[i].point));

      //------

      x1 = x2;
      y1 = y2;
      x2 = parts_[i].point.x;
      y2 = parts_[i].point.y;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      if (getStrokeAdjust())
        stroker.adjustPoint(parts_[i + 2].point);

      if (i > 0 && (parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_LINE_TO ||
                    parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
                    parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO))
        stroker.joinLines(CPoint2D(x1, y1), CPoint2D(x2, y2), parts_[i].point);

      stroker.drawBezier2(CPoint2D(x2, y2), parts_[i].point, parts_[i + 1].point);

      i += 1;

      //------

      x1 = parts_[i - 1].point.x;
      y1 = parts_[i - 1].point.y;
      x2 = parts_[i    ].point.x;
      y2 = parts_[i    ].point.y;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      if (getStrokeAdjust())
        stroker.adjustPoint(parts_[i + 2].point);

      if (i > 0 && (parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_LINE_TO ||
                    parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
                    parts_[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO))
        stroker.joinLines(CPoint2D(x1, y1), CPoint2D(x2, y2), parts_[i].point);

      stroker.drawBezier3(CPoint2D(x2, y2), parts_[i].point, parts_[i + 1].point, parts_[i + 2].point);

      i += 2;

      //------

      x1 = parts_[i - 1].point.x;
      y1 = parts_[i - 1].point.y;
      x2 = parts_[i    ].point.x;
      y2 = parts_[i    ].point.y;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH) {
      if (i - start_i > 1) {
        if (fabs(x2 - parts_[start_i].point.x) > 1E-5 ||
            fabs(y2 - parts_[start_i].point.y) > 1E-5) {
          if (i - start_i > 2)
            stroker.joinLines(CPoint2D(x1, y1), CPoint2D(x2, y2), parts_[start_i].point);

          stroker.drawLine(CPoint2D(x2, y2), parts_[start_i].point);

          if (i - start_i > 2)
            stroker.joinLines(CPoint2D(x2, y2), parts_[start_i].point, parts_[start_i + 1].point);
        }
        else {
          if (i - start_i > 2)
            stroker.joinLines(CPoint2D(x1, y1), parts_[start_i].point, parts_[start_i + 1].point);
        }
      }
    }
  }

  if (num_parts > 1 && ! closed) {
    if (parts_[num_parts - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
        parts_[num_parts - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO)
      stroker.capLine(CPoint2D(x2, y2), CPoint2D(x1, y1));
    else
      stroker.capLine(CPoint2D(x2, y2), parts_[start_i].point);
  }

  setStrokeAdjust(false);
}

void
CPath2D::
strokePath()
{
  strokePath(renderer_);
}

void
CPath2D::
strokePath(CPath2DRenderer *renderer)
{
  if (! renderer) return;

  uint num_parts = parts_.size();

  PartList parts1;

  parts1.resize(num_parts);

  for (uint i = 0; i < num_parts; i++)
    parts1[i] = parts_[i];

  //------

  init();

  renderer->setTransform(false);

  double x1 = 0.0;
  double y1 = 0.0;
  double x2 = 0.0;
  double y2 = 0.0;

  bool closed = false;

  int start_i = 0;

  for (uint i = 0; i < num_parts; i++) {
    if      (parts1[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO) {
      if (getStrokeAdjust())
        renderer->adjustPoint(parts1[i].point);

      if (i - start_i > 1 && ! closed) {
        if (parts1[start_i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
            parts1[start_i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO)
          strokeCapLine(renderer, CPoint2D(x2, y2), CPoint2D(x1, y1));
        else
          strokeCapLine(renderer, CPoint2D(x2, y2), parts1[start_i].point);
      }

      //------

      closed = false;

      uint j = i + 1;

      while (j < num_parts) {
        if (parts1[j].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH) {
          closed = true;
          break;
        }

        j++;
      }

      //------

      if (! closed) {
        if (j - i > 1)
          strokeCapLine(renderer, parts1[i].point, parts1[i + 1].point);
        else
          strokeCapLine(renderer, parts1[i].point, parts1[i    ].point);
      }

      //------

      start_i = i;

      x2 = parts1[i].point.x;
      y2 = parts1[i].point.y;
    }
    else if (parts1[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
      if (getStrokeAdjust())
        renderer->adjustPoint(parts1[i].point);

      if (i > 0 && (parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_LINE_TO ||
                    parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
                    parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO))
        strokeJoinLines(renderer, CPoint2D(x1, y1), CPoint2D(x2, y2), parts1[i].point);

      strokeLine(renderer, CPoint2D(x2, y2), parts1[i].point);

      //------

      x1 = x2;
      y1 = y2;
      x2 = parts1[i].point.x;
      y2 = parts1[i].point.y;
    }
    else if (parts1[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      if (getStrokeAdjust())
        renderer->adjustPoint(parts1[i + 2].point);

      if (i > 0 && (parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_LINE_TO ||
                    parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
                    parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO))
        strokeJoinLines(renderer, CPoint2D(x1, y1), CPoint2D(x2, y2), parts1[i].point);

      strokeBezier2(renderer, CPoint2D(x2, y2), parts1[i].point, parts1[i + 1].point);

      i += 1;

      //------

      x1 = parts1[i - 1].point.x;
      y1 = parts1[i - 1].point.y;
      x2 = parts1[i    ].point.x;
      y2 = parts1[i    ].point.y;
    }
    else if (parts1[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      if (getStrokeAdjust())
        renderer->adjustPoint(parts1[i + 2].point);

      if (i > 0 && (parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_LINE_TO ||
                    parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
                    parts1[i - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO))
        strokeJoinLines(renderer, CPoint2D(x1, y1), CPoint2D(x2, y2), parts1[i].point);

      strokeBezier3(renderer, CPoint2D(x2, y2), parts1[i].point, parts1[i + 1].point, parts1[i + 2].point);

      i += 2;

      //------

      x1 = parts1[i - 1].point.x;
      y1 = parts1[i - 1].point.y;
      x2 = parts1[i    ].point.x;
      y2 = parts1[i    ].point.y;
    }
    else if (parts1[i].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH) {
      if (i - start_i > 1) {
        if (fabs(x2 - parts1[start_i].point.x) > 1E-5 ||
            fabs(y2 - parts1[start_i].point.y) > 1E-5) {
          if (i - start_i > 2)
            strokeJoinLines(renderer, CPoint2D(x1, y1), CPoint2D(x2, y2), parts1[start_i].point);

          strokeLine(renderer, CPoint2D(x2, y2), parts1[start_i].point);

          if (i - start_i > 2)
            strokeJoinLines(renderer, CPoint2D(x2, y2), parts1[start_i].point, parts1[start_i + 1].point);
        }
        else {
          if (i - start_i > 2)
            strokeJoinLines(renderer, CPoint2D(x1, y1), parts1[start_i].point, parts1[start_i + 1].point);
        }
      }
    }
  }

  if (num_parts > 1 && ! closed) {
    if (parts1[num_parts - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO ||
        parts1[num_parts - 1].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO)
      strokeCapLine(renderer, CPoint2D(x2, y2), CPoint2D(x1, y1));
    else
      strokeCapLine(renderer, CPoint2D(x2, y2), parts1[start_i].point);
  }

  renderer->setTransform(true);

  setStrokeAdjust(false);
}

void
CPath2D::
strokeBezier2(CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2,
              const CPoint2D &point3)
{
  C2Bezier2D bezier(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y);

  std::vector<CPoint2D> points;
  std::vector<double>   gradients;

  if (flattener_)
    flattener_->bezierToLines(bezier, points, gradients);
  else
    CMathGeom2D::BezierToLines(bezier, points);

  uint num_points = points.size();

  for (uint i = 0; i < num_points - 1; i++)
    strokeLine(renderer, points[i], points[i + 1]);
}

void
CPath2D::
strokeBezier3(CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2,
              const CPoint2D &point3, const CPoint2D &point4)
{
  C3Bezier2D bezier(point1.x, point1.y, point2.x, point2.y,
                    point3.x, point3.y, point4.x, point4.y);

  std::vector<CPoint2D> points;
  std::vector<double>   gradients;

  if (flattener_)
    flattener_->bezierToLines(bezier, points, gradients);
  else
    CMathGeom2D::BezierToLines(bezier, points);

  uint num_points = points.size();

  for (uint i = 0; i < num_points - 1; i++)
    strokeLine(renderer, points[i], points[i + 1]);
}

void
CPath2D::
strokeLine(CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2)
{
  if (renderer)
    renderer->setTransform(false);

  PointList points;

  if (flattener_)
    flattener_->lineToPolygon(point1, point2, points);
  else
    CMathGeom2D::LineToPolygon(point1, point2, points, 1.0);

  uint num_points = points.size();

  for (uint i = 0; i < num_points; i++) {
    if (i == 0)
      moveTo(points[i]);
    else
      lineTo(points[i]);

    if (i == num_points - 1)
      close();
  }

  if (renderer)
    renderer->setTransform(true);
}

void
CPath2D::
strokeCapLine(CPath2DRenderer *, const CPoint2D &, const CPoint2D &)
{
}

void
CPath2D::
strokeJoinLines(CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2,
                const CPoint2D &point3)
{
  if (renderer)
    renderer->setTransform(false);

  PointList points;

  if (flattener_)
    flattener_->lineJoinToPolygon(point1, point2, point3, points);
  else
    CMathGeom2D::LineJoinToPolygon(point1, point2, point3, LINE_JOIN_TYPE_MITRE, points);

  uint num_points = points.size();

  for (uint i = 0; i < num_points; i++) {
    if (i == 0)
      moveTo(points[i]);
    else
      lineTo(points[i]);

    if (i == num_points - 1)
      close();
  }

  if (renderer)
    renderer->setTransform(true);
}

void
CPath2D::
setFillType(CFillType fill_type)
{
  fill_type_ = fill_type;
}

CFillType
CPath2D::
getFillType() const
{
  return fill_type_;
}

void
CPath2D::
fill()
{
  CPath2D path1(*this);

  CImagePtr image;

  path1.subFill(image, fill_type_, renderer_);
}

void
CPath2D::
fill(CPath2DRenderer *renderer)
{
  CPath2D path1(*this);

  CImagePtr image;

  path1.subFill(image, fill_type_, renderer);
}

void
CPath2D::
fill(CPath2DFiller &filler)
{
  subShapeFill(fill_type_, filler);
}

void
CPath2D::
fillImage(CImagePtr image)
{
  CPath2D path1(*this);

  path1.subFill(image, fill_type_, renderer_);
}

void
CPath2D::
fillImage(CImagePtr image, CPath2DRenderer *renderer)
{
  CPath2D path1(*this);

  path1.subFill(image, fill_type_, renderer);
}

void
CPath2D::
fillGradient(CRefPtr<CGenGradient>)
{
  CPath2D path1(*this);

  CImagePtr image;

  path1.subFill(image, fill_type_, renderer_);
}

void
CPath2D::
subFill(CImagePtr image, CFillType type, CPath2DRenderer *renderer)
{
  if (! renderer) return;

  renderer->setTransform(false);

  subFill1(image, type, renderer);

  renderer->setTransform(true);
}

void
CPath2D::
subFill1(CImagePtr image, CFillType type, CPath2DRenderer *renderer)
{
  if (flat_fill_)
    subFlatFill(image, type, renderer);
  else
    subShapeFill(type, renderer);
}

void
CPath2D::
subFlatFill(CImagePtr image, CFillType type, CPath2DRenderer *renderer)
{
  flatten();

  uint num_parts = parts_.size();

  if (num_parts == 0)
    return;

  //------

  renderer->initPolygons();

  //------

  fill_points_.clear();

  bool closed = false;

  for (uint i = 0; i < num_parts; i++) {
    if      (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO) {
      if (! closed && fill_points_.size() > 2)
        renderer->addPolygon(fill_points_);

      fill_points_.clear();

      fill_points_.push_back(parts_[i].point);

      closed = false;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
      fill_points_.push_back(parts_[i].point);

      closed = false;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH) {
      if (! closed && fill_points_.size() > 2)
        renderer->addPolygon(fill_points_);

      fill_points_.clear();

      closed = true;
    }
  }

  if (! closed && fill_points_.size() > 2)
    renderer->addPolygon(fill_points_);

  if (image.isValid()) {
    if (type == FILL_TYPE_WINDING)
      renderer->imageFillPolygons(image);
    else
      renderer->imageEoFillPolygons(image);
  }
  else {
    if (type == FILL_TYPE_WINDING)
      renderer->fillPolygons();
    else
      renderer->eofillPolygons();
  }
}

void
CPath2D::
subShapeFill(CFillType type, CPath2DRenderer *renderer)
{
  CPath2DRendererFiller filler(renderer);

  subShapeFill(type, filler);
}

void
CPath2D::
subShapeFill(CFillType type, CPath2DFiller &filler)
{
  double x2 = 0.0;
  double y2 = 0.0;

  //------

  bool closed = false;

  int start_i = 0;

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; i++) {
    if      (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO) {
      if (! closed)
        filler.fill(type);

      closed = true;

      //------

      start_i = i;

      x2 = parts_[i].point.x;
      y2 = parts_[i].point.y;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
      filler.addLine(CPoint2D(x2, y2), CPoint2D(parts_[i].point));

      closed = false;

      //------

      x2 = parts_[i].point.x;
      y2 = parts_[i].point.y;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      filler.addBezier2(CPoint2D(x2, y2), parts_[i].point, parts_[i + 1].point);

      closed = false;

      i += 1;

      //------

      x2 = parts_[i].point.x;
      y2 = parts_[i].point.y;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      filler.addBezier3(CPoint2D(x2, y2), parts_[i].point,
                        parts_[i + 1].point, parts_[i + 2].point);

      closed = false;

      i += 2;

      //------

      x2 = parts_[i].point.x;
      y2 = parts_[i].point.y;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH) {
      if (i - start_i > 1) {
        if (fabs(x2 - parts_[start_i].point.x) > 1E-5 ||
            fabs(y2 - parts_[start_i].point.y) > 1E-5) {
          filler.addLine(CPoint2D(x2, y2), parts_[start_i].point);

          closed = false;
        }
      }

      if (! closed)
        filler.fill(type);

      closed = true;
    }
  }
}

void
CPath2D::
reverse()
{
  uint num_parts = parts_.size();

  PartList parts;

  parts.resize(num_parts);

  uint start_i = 0;

  while (start_i < num_parts) {
    int i = (int) start_i;

    if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO)
      i++;

    while (i < (int) num_parts &&
           parts_[i].type != CPATH_2D_PATH_PART_TYPE_CLOSE_PATH &&
           parts_[i].type != CPATH_2D_PATH_PART_TYPE_MOVE_TO)
      i++;

    if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO)
      i--;

    //------

    bool closed = false;

    if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH) {
      i--;

      closed = true;
    }

    //------

    uint end_i = i;

    while (i >= (int) start_i) {
      if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO ||
          parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
        if (i == (int) end_i)
          parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_MOVE_TO, parts_[i].point));
        else
          parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_LINE_TO, parts_[i].point));
      }
      else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
        if (i == (int) end_i) {
          parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_MOVE_TO, parts_[i].point));

          i--;
        }

        //------

        parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER2_TO, parts_[i].point));

        i--;

        parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER2_TO, parts_[i].point));

        //------

        if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
          i--;

          //------

          parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_LINE_TO, parts_[i].point));
        }
      }
      else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
        if (i == (int) end_i) {
          parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_MOVE_TO, parts_[i].point));

          i--;
        }

        //------

        parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER3_TO, parts_[i].point));

        i--;

        parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER3_TO, parts_[i].point));

        i--;

        parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_BEZIER3_TO, parts_[i].point));

        //------

        if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
          i--;

          //------

          parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_LINE_TO, parts_[i].point));
        }
      }

      i--;
    }

    if (closed) {
      parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_CLOSE_PATH, parts_[i].point));

      start_i = end_i + 2;
    }
    else
      start_i = end_i + 1;
  }

  uint len = parts.size();

  parts_.resize(len);

  for (uint i = 0; i < len; ++i)
    parts_[i] = parts[i];
}

void
CPath2D::
flatten()
{
  flatten(flattener_);
}

void
CPath2D::
flatten(CPath2DFlattener *flattener)
{
  uint num_parts = parts_.size();

  if (num_parts == 0)
    return;

  PartList parts;

  parts.resize(num_parts);

  std::vector<CPoint2D> points;
  std::vector<double>   gradients;

  for (uint i = 0; i < num_parts; i++) {
    if      (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      C2Bezier2D bezier(parts_[i - 1].point.x, parts_[i - 1].point.y,
                        parts_[i    ].point.x, parts_[i    ].point.y,
                        parts_[i + 1].point.x, parts_[i + 1].point.y);

      if (flattener)
        flattener->bezierToLines(bezier, points, gradients);
      else
        CMathGeom2D::BezierToLines(bezier, points);

      uint num_points = points.size();

      if (num_points > 1) {
        for (uint k = 1; k < num_points; k++)
          parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_LINE_TO, points[k]));
      }
      else
        parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_LINE_TO, parts_[i + 2].point));

      i += 1;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      C3Bezier2D bezier(parts_[i - 1].point.x, parts_[i - 1].point.y,
                        parts_[i    ].point.x, parts_[i    ].point.y,
                        parts_[i + 1].point.x, parts_[i + 1].point.y,
                        parts_[i + 2].point.x, parts_[i + 2].point.y);

      if (flattener)
        flattener->bezierToLines(bezier, points, gradients);
      else
        CMathGeom2D::BezierToLines(bezier, points);

      uint num_points = points.size();

      if (num_points > 1) {
        for (uint k = 1; k < num_points; k++)
          parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_LINE_TO, points[k]));
      }
      else
        parts.push_back(CPath2DPart(CPATH_2D_PATH_PART_TYPE_LINE_TO, parts_[i + 2].point));

      i += 2;
    }
    else
      parts.push_back(parts_[i]);
  }

  uint len = parts.size();

  parts_.resize(len);

  for (uint i = 0; i < len; ++i)
    parts_[i] = parts[i];
}

void
CPath2D::
clip()
{
  subClip(FILL_TYPE_WINDING);
}

void
CPath2D::
eoclip()
{
  subClip(FILL_TYPE_EVEN_ODD);
}

void
CPath2D::
subClip(CFillType type)
{
  if (! renderer_) return;

  renderer_->setTransform(false);

  subClip(type, renderer_);

  renderer_->setTransform(true);
}

void
CPath2D::
subClip(CFillType type, CPath2DRenderer *renderer)
{
  flatten(flattener_);

  uint num_parts = parts_.size();

  if (num_parts == 0)
    return;

  //------

  renderer->initPolygons();

  //------

  fill_points_.clear();

  bool closed = false;

  for (uint i = 0; i < num_parts; i++) {
    if      (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO) {
      if (! closed && fill_points_.size() > 2)
        renderer->addPolygon(fill_points_);

      fill_points_.clear();

      fill_points_.push_back(parts_[i].point);

      closed = false;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
      fill_points_.push_back(parts_[i].point);

      closed = false;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH) {
      if (! closed && fill_points_.size() > 2)
        renderer->addPolygon(fill_points_);

      fill_points_.clear();

      closed = true;
    }
  }

  if (! closed && fill_points_.size() > 2)
    renderer->addPolygon(fill_points_);

  if (type == FILL_TYPE_WINDING)
    renderer->clipPolygons();
  else
    renderer->eoclipPolygons();
}

void
CPath2D::
clipPath()
{
  if (! renderer_) return;

  init();

  uint num_clips = renderer_->getNumClipPolygons();

  for (uint i = 1; i <= num_clips; ++i) {
    const std::vector< std::vector<CPoint2D> > &polygon_points = renderer_->getClipPolygons(i);

    uint num_polygon_points = polygon_points.size();

    for (uint j = 0; j < num_polygon_points; ++j) {
      const PointList &points = polygon_points[j];

      uint num_points = points.size();

      for (uint k = 0; k < num_points; ++k) {
        if (k == 0)
          moveTo(points[k]);
        else
          lineTo(points[k]);
      }

      close();
    }
  }
}

void
CPath2D::
getBounds(double *xmin, double *ymin, double *xmax, double *ymax)
{
  uint num_parts = parts_.size();

  *xmin =  1E20;
  *ymin =  1E20;
  *xmax = -1E20;
  *ymax = -1E20;

  for (uint i = 0; i < num_parts; i++) {
    if       (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO) {
      if (parts_[i].point.x < *xmin) *xmin = parts_[i].point.x;
      if (parts_[i].point.y < *ymin) *ymin = parts_[i].point.y;
      if (parts_[i].point.x > *xmax) *xmax = parts_[i].point.x;
      if (parts_[i].point.y > *ymax) *ymax = parts_[i].point.y;
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
      if (parts_[i].point.x < *xmin) *xmin = parts_[i].point.x;
      if (parts_[i].point.y < *ymin) *ymin = parts_[i].point.y;
      if (parts_[i].point.x > *xmax) *xmax = parts_[i].point.x;
      if (parts_[i].point.y > *ymax) *ymax = parts_[i].point.y;
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      if (parts_[i    ].point.x < *xmin) *xmin = parts_[i    ].point.x;
      if (parts_[i    ].point.y < *ymin) *ymin = parts_[i    ].point.y;
      if (parts_[i    ].point.x > *xmax) *xmax = parts_[i    ].point.x;
      if (parts_[i    ].point.y > *ymax) *ymax = parts_[i    ].point.y;

      if (parts_[i + 1].point.x < *xmin) *xmin = parts_[i + 1].point.x;
      if (parts_[i + 1].point.y < *ymin) *ymin = parts_[i + 1].point.y;
      if (parts_[i + 1].point.x > *xmax) *xmax = parts_[i + 1].point.x;
      if (parts_[i + 1].point.y > *ymax) *ymax = parts_[i + 1].point.y;

      i += 1;
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      if (parts_[i    ].point.x < *xmin) *xmin = parts_[i    ].point.x;
      if (parts_[i    ].point.y < *ymin) *ymin = parts_[i    ].point.y;
      if (parts_[i    ].point.x > *xmax) *xmax = parts_[i    ].point.x;
      if (parts_[i    ].point.y > *ymax) *ymax = parts_[i    ].point.y;

      if (parts_[i + 1].point.x < *xmin) *xmin = parts_[i + 1].point.x;
      if (parts_[i + 1].point.y < *ymin) *ymin = parts_[i + 1].point.y;
      if (parts_[i + 1].point.x > *xmax) *xmax = parts_[i + 1].point.x;
      if (parts_[i + 1].point.y > *ymax) *ymax = parts_[i + 1].point.y;

      if (parts_[i + 2].point.x < *xmin) *xmin = parts_[i + 2].point.x;
      if (parts_[i + 2].point.y < *ymin) *ymin = parts_[i + 2].point.y;
      if (parts_[i + 2].point.x > *xmax) *xmax = parts_[i + 2].point.x;
      if (parts_[i + 2].point.y > *ymax) *ymax = parts_[i + 2].point.y;

      i += 2;
    }
  }
}

void
CPath2D::
process(CPath2DVisitor &visitor) const
{
  CMatrix2D matrix;

  if (renderer_) {
    const CMatrix2D *imatrix = renderer_->getIViewMatrix();

    if (imatrix == NULL)
      return;

    matrix = *imatrix;
  }
  else
    matrix.setIdentity();

  //------

  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; i++) {
    if       (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO) {
      CPoint2D p;

      matrix.multiplyPoint(parts_[i].point, p);

      visitor.moveTo(p);
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
      CPoint2D p;

      matrix.multiplyPoint(parts_[i].point, p);

      visitor.lineTo(p);
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      CPoint2D p1, p2;

      matrix.multiplyPoint(parts_[i    ].point, p1);
      matrix.multiplyPoint(parts_[i + 1].point, p2);

      visitor.bezier2To(p1, p2);

      i += 1;
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      CPoint2D p1, p2, p3;

      matrix.multiplyPoint(parts_[i    ].point, p1);
      matrix.multiplyPoint(parts_[i + 1].point, p2);
      matrix.multiplyPoint(parts_[i + 2].point, p3);

      visitor.bezier3To(p1, p2, p3);

      i += 2;
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH)
      visitor.close();
  }
}

void
CPath2D::
bbox(CBBox2D &bbox) const
{
  uint num_parts = parts_.size();

  if (num_parts == 0)
    return;

  //------

  bbox.reset();

  for (uint i = 0; i < num_parts; i++) {
    if      (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO)
      bbox.add(parts_[i].point);
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO)
      bbox.add(parts_[i].point);
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      bbox.add(parts_[i + 0].point);
      bbox.add(parts_[i + 1].point);

      i += 1;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      bbox.add(parts_[i + 0].point);
      bbox.add(parts_[i + 1].point);
      bbox.add(parts_[i + 2].point);

      i += 2;
    }
  }
}

void
CPath2D::
transform(const CMatrix2D &matrix)
{
  uint num_parts = parts_.size();

  if (num_parts == 0)
    return;

  //------

  CPoint2D t;

  for (uint i = 0; i < num_parts; i++) {
    if      (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO) {
      matrix.multiplyPoint(parts_[i].point, t); parts_[i].point = t;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO) {
      matrix.multiplyPoint(parts_[i].point, t); parts_[i].point = t;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      matrix.multiplyPoint(parts_[i + 0].point, t); parts_[i + 0].point = t;
      matrix.multiplyPoint(parts_[i + 1].point, t); parts_[i + 1].point = t;

      i += 1;
    }
    else if (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      matrix.multiplyPoint(parts_[i + 0].point, t); parts_[i + 0].point = t;
      matrix.multiplyPoint(parts_[i + 1].point, t); parts_[i + 1].point = t;
      matrix.multiplyPoint(parts_[i + 2].point, t); parts_[i + 2].point = t;

      i += 2;
    }
  }
}

void
CPath2D::
transformPoint(const CPoint2D &point, CPoint2D &point1)
{
  if (renderer_)
    renderer_->transformPoint(point, point1);
  else
    point1 = point;
}

void
CPath2D::
untransformPoint(const CPoint2D &point, CPoint2D &point1)
{
  if (renderer_)
    renderer_->untransformPoint(point, point1);
  else
    point1 = point;
}

void
CPath2D::
print()
{
  uint num_parts = parts_.size();

  for (uint i = 0; i < num_parts; i++) {
    if       (parts_[i].type == CPATH_2D_PATH_PART_TYPE_MOVE_TO)
      CStrUtil::printf("Move To %g,%g\n", parts_[i].point.x, parts_[i].point.y);
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_LINE_TO)
      CStrUtil::printf("Line To %g,%g\n", parts_[i].point.x, parts_[i].point.y);
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER2_TO) {
      CStrUtil::printf("Bezier2 To %g,%g %g,%g %g,%g\n",
                       parts_[i    ].point.x, parts_[i    ].point.y,
                       parts_[i + 1].point.x, parts_[i + 1].point.y);

      i += 1;
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_BEZIER3_TO) {
      CStrUtil::printf("Bezier3 To %g,%g %g,%g %g,%g\n",
                       parts_[i    ].point.x, parts_[i    ].point.y,
                       parts_[i + 1].point.x, parts_[i + 1].point.y,
                       parts_[i + 2].point.x, parts_[i + 2].point.y);

      i += 2;
    }
    else if  (parts_[i].type == CPATH_2D_PATH_PART_TYPE_CLOSE_PATH)
      CStrUtil::printf("Close Path\n");
    else
      CStrUtil::printf("??\n");
  }
}

//--------------

void
CPath2DRendererStroker::
adjustPoint(CPoint2D &p1)
{
  renderer_->adjustPoint(p1);
}

void
CPath2DRendererStroker::
capLine(const CPoint2D &p1, const CPoint2D &p2)
{
  renderer_->capLine(p1, p2);
}

void
CPath2DRendererStroker::
joinLines(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  renderer_->joinLines(p1, p2, p3);
}

void
CPath2DRendererStroker::
drawLine(const CPoint2D &p1, const CPoint2D &p2)
{
  renderer_->drawLine(p1, p2);
}

void
CPath2DRendererStroker::
drawBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  renderer_->drawBezier2(p1, p2, p3);
}

void
CPath2DRendererStroker::
drawBezier3(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4)
{
  renderer_->drawBezier3(p1, p2, p3, p4);
}

//--------------

void
CPath2DRendererFiller::
addLine(const CPoint2D &, const CPoint2D &)
{
}

void
CPath2DRendererFiller::
addBezier2(const CPoint2D &, const CPoint2D &, const CPoint2D &)
{
}

void
CPath2DRendererFiller::
addBezier3(const CPoint2D &, const CPoint2D &, const CPoint2D &, const CPoint2D &)
{
}

void
CPath2DRendererFiller::
fill(CFillType)
{
}

//--------------

void
CPath2DRenderer::
getPixelSize(double *px, double *py)
{
  *px = 1.0;
  *py = 1.0;
}

void
CPath2DRenderer::
setTransform(bool)
{
}

void
CPath2DRenderer::
adjustPoint(CPoint2D &)
{
}

void
CPath2DRenderer::
transformPoint(const CPoint2D &p1, CPoint2D &p2)
{
  p2 = p1;
}

void
CPath2DRenderer::
untransformPoint(const CPoint2D &p1, CPoint2D &p2)
{
  p2 = p1;
}

void
CPath2DRenderer::
drawBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  C2Bezier2D bezier(p1, p2, p3);

  std::vector<CPoint2D> points;
  std::vector<double>   gradients;

  CMathGeom2D::BezierToLines(bezier, points);

  uint num_points = points.size();

  for (uint i = 0; i < num_points - 1; ++i)
    drawLine(points[i], points[i + 1]);
}

void
CPath2DRenderer::
drawBezier3(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4)
{
  C3Bezier2D bezier(p1, p2, p3, p4);

  std::vector<CPoint2D> points;
  std::vector<double>   gradients;

  double px, py;

  getPixelSize(&px, &py);

  double tol = std::max(px, py)*4;

  CMathGeom2D::BezierToLines(bezier, points, tol);

  uint num_points = points.size();

  for (uint i = 0; i < num_points - 1; ++i)
    drawLine(points[i], points[i + 1]);
}

void
CPath2DRenderer::
drawText(const CPoint2D &, const std::string &)
{
}

void
CPath2DRenderer::
setTextSize(double, double)
{
}

void
CPath2DRenderer::
getTextLimits(const std::string &, double *, double *)
{
}

void
CPath2DRenderer::
capLine(const CPoint2D &, const CPoint2D &)
{
}

void
CPath2DRenderer::
joinLines(const CPoint2D &, const CPoint2D &, const CPoint2D &)
{
}

void
CPath2DRenderer::
initPolygons()
{
  polygons_.clear();
}

void
CPath2DRenderer::
addPolygon(const std::vector<CPoint2D> &points)
{
  polygons_.push_back(points);
}

void
CPath2DRenderer::
eofillPolygons()
{
  fillPolygons();
}

void
CPath2DRenderer::
imageFillPolygons(CImagePtr)
{
  assert(false);
}

void
CPath2DRenderer::
imageEoFillPolygons(CImagePtr)
{
  assert(false);
}

uint
CPath2DRenderer::
getNumClipPolygons() const
{
  return 0;
}

const std::vector< std::vector<CPoint2D> > &
CPath2DRenderer::
getClipPolygons(uint)
{
  assert(false);
}

void
CPath2DRenderer::
clipPolygons()
{
}

void
CPath2DRenderer::
eoclipPolygons()
{
}

const CMatrix2D *
CPath2DRenderer::
getIViewMatrix()
{
  return NULL;
}

//-----------------

void
CPath2DFlattener::
arcToBeziers(const CPoint2D &c, double xr, double yr, double a1, double a2,
             std::vector<C3Bezier2D> &beziers)
{
  CMathGeom2D::ArcToBeziers(c.x, c.y, xr, yr, a1, a2, beziers);
}

void
CPath2DFlattener::
arcNToBeziers(const CPoint2D &c, double xr, double yr, double a1, double a2,
              std::vector<C3Bezier2D> &beziers)
{
  CMathGeom2D::ArcNToBeziers(c.x, c.y, xr, yr, a1, a2, beziers);
}

void
CPath2DFlattener::
bezierToLines(const C2Bezier2D &bezier, std::vector<CPoint2D> &points, std::vector<double> &)
{
  CMathGeom2D::BezierToLines(bezier, points, getLineTolerance());
}

void
CPath2DFlattener::
bezierToLines(const C3Bezier2D &bezier, std::vector<CPoint2D> &points, std::vector<double> &)
{
  CMathGeom2D::BezierToLines(bezier, points, getLineTolerance());
}

void
CPath2DFlattener::
lineToPolygon(const CPoint2D &p1, const CPoint2D &p2, std::vector<CPoint2D> &points)
{
  CMathGeom2D::LineToPolygon(p1, p2, points, 1.0);
}

void
CPath2DFlattener::
lineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                  std::vector<CPoint2D> &points)
{
  CMathGeom2D::LineJoinToPolygon(p1, p2, p3, LINE_JOIN_TYPE_MITRE, points);
}

double
CPath2DFlattener::
getLineTolerance() const
{
  return -1.0;
}
