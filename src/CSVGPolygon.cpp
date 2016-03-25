#include <CSVGPolygon.h>
#include <CSVG.h>
#include <CSVGLog.h>

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
    points    <Points>
    transform <TransformList>
*/

CSVGPolygon::
CSVGPolygon(CSVG &svg) :
 CSVGObject(svg),
 points_   ()
{
}

CSVGPolygon::
CSVGPolygon(const CSVGPolygon &poly) :
 CSVGObject(poly),
 points_   (poly.points_)
{
}

CSVGPolygon *
CSVGPolygon::
dup() const
{
  return new CSVGPolygon(*this);
}

bool
CSVGPolygon::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processPaintOption          (opt_name, opt_value)) return true;
  if (processColorOption          (opt_name, opt_value)) return true;
  if (processOpacityOption        (opt_name, opt_value)) return true;
  if (processGraphicsOption       (opt_name, opt_value)) return true;
  if (processMarkerOption         (opt_name, opt_value)) return true;
  if (processClipOption           (opt_name, opt_value)) return true;
  if (processMaskOption           (opt_name, opt_value)) return true;
  if (processFilterOption         (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processCursorOption         (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  std::vector<CPoint2D> points;
  std::string           str;

  if      (svg_.pointListOption(opt_name, opt_value, "points", points)) {
    points_.clear();

    uint num_points = points.size();

    for (uint i = 0; i < num_points; ++i)
      addPoint(points[i]);
  }
  else if (svg_.stringOption(opt_name, opt_value, "transform", str)) {
    CMatrix2D transform;

    if (! svg_.decodeTransform(str, transform))
      return false;

    setTransform(transform);
  }
  else
    return false;

  return true;
}

void
CSVGPolygon::
addPoint(const CPoint2D &point)
{
  points_.push_back(point);
}

void
CSVGPolygon::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  if (svg_.isFilled())
    svg_.fillPolygon(points_);

  if (svg_.isStroked())
    svg_.drawPolygon(points_);
}

bool
CSVGPolygon::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isSet()) {
    CBBox2D bbox1;

    uint num_points = points_.size();

    for (uint i = 0; i < num_points; ++i)
      bbox1.add(points_[i]);

    bbox = bbox1;
  }
  else
    bbox = viewBox_;

  return true;
}

void
CSVGPolygon::
moveBy(const CVector2D &delta)
{
  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i)
    points_[i] += delta;
}

void
CSVGPolygon::
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
CSVGPolygon::
rotateBy(double da, const CPoint2D &c)
{
  uint num_points = points_.size();

  for (uint i = 0; i < num_points; ++i)
    points_[i] = points_[i].rotate(c, da);
}

void
CSVGPolygon::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<polygon points=\"";

    for (uint i = 0; i < points_.size(); ++i) {
      if (i > 0)
        os << " ";

      os << points_[i].x << "," << points_[i].y;
    }

    os << "\"";

    printStyle(os);

    os << "/>" << std::endl;
  }
  else {
    os << "polygon ";

    for (uint i = 0; i < points_.size(); ++i) {
      if (i > 0)
        os << " ";

      os << points_[i];
    }
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGPolygon &polygon)
{
  polygon.print(os, false);

  return os;
}
