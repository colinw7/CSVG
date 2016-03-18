#include <CSVGI.h>

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Paint>
    <Color>
    <Opacity>
    <Graphics>
    <Marker>
    <Clip>
    <Mask>
    <Filter>
    <GraphicalEvents>
    <Cursor>
    <External>
    points <Points>
    transform <TransformList>
*/

CSVGPolyLine::
CSVGPolyLine(CSVG &svg) :
 CSVGObject(svg),
 points_   ()
{
}

CSVGPolyLine::
CSVGPolyLine(const CSVGPolyLine &poly) :
 CSVGObject(poly),
 points_   (poly.points_)
{
}

CSVGPolyLine *
CSVGPolyLine::
dup() const
{
  return new CSVGPolyLine(*this);
}

bool
CSVGPolyLine::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::vector<CPoint2D> points;

  if      (svg_.pointListOption(opt_name, opt_value, "points", points)) {
    points_.clear();

    uint num_points = points.size();

    for (uint i = 0; i < num_points; ++i)
      addPoint(points[i]);
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGPolyLine::
addPoint(const CPoint2D &point)
{
  points_.push_back(point);
}

void
CSVGPolyLine::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.pathInit();

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i) {
    if (i == 0)
      svg_.pathMoveTo(points_[i].x, points_[i].y);
    else
      svg_.pathLineTo(points_[i].x, points_[i].y);
  }

  if (svg_.isFilled())
    svg_.pathFill();

  if (svg_.isStroked())
    svg_.pathStroke();
}

bool
CSVGPolyLine::
getBBox(CBBox2D &bbox) const
{
  if (! view_box_.isSet()) {
    CBBox2D bbox1;

    uint num_points = points_.size();

    for (uint i = 0; i < num_points; ++i)
      bbox1.add(points_[i]);

    bbox = bbox1;
  }
  else
    bbox = view_box_;

  return true;
}

void
CSVGPolyLine::
moveBy(const CVector2D &delta)
{
  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i)
    points_[i] += delta;
}

void
CSVGPolyLine::
resizeTo(const CSize2D &size)
{
  CBBox2D bbox;

  getBBox(bbox);

  double x1 = bbox.getXMin();
  double y1 = bbox.getYMin();

  double w1 = bbox.getWidth ();
  double h1 = bbox.getHeight();

  double w2 = size.getWidth ();
  double h2 = size.getHeight();

  double sx = (w1 > 0 ? w2/w1 : 1.0);
  double sy = (h1 > 0 ? h2/h1 : 1.0);

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i) {
    points_[i].x = (points_[i].x - x1)*sx + x1;
    points_[i].y = (points_[i].y - y1)*sy + y1;
  }
}

void
CSVGPolyLine::
rotateBy(double da, const CPoint2D &c)
{
  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i)
    points_[i] = points_[i].rotate(c, da);
}

void
CSVGPolyLine::
print(std::ostream &os) const
{
  os << "polyline ";

  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i) {
    if (i > 0)
      os << " ";

    os << points_[i];
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGPolyLine &polyline)
{
  polyline.print(os);

  return os;
}
