#include <CSVGLine.h>
#include <CSVG.h>
#include <CSVGBuffer.h>
#include <CSVGLog.h>
#include <CMathGeom2D.h>

CSVGLine::
CSVGLine(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGLine::
CSVGLine(const CSVGLine &line) :
 CSVGObject(line),
 x1_       (line.x1_),
 y1_       (line.y1_),
 x2_       (line.x2_),
 y2_       (line.y2_)
{
}

CSVGLine *
CSVGLine::
dup() const
{
  return new CSVGLine(*this);
}

bool
CSVGLine::
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

  std::string    str;
  CScreenUnits   length;
  CMatrixStack2D transform;

  if      (svg_.coordOption    (opt_name, opt_value, "x1", length))
    x1_ = length;
  else if (svg_.coordOption    (opt_name, opt_value, "y1", length))
    y1_ = length;
  else if (svg_.coordOption    (opt_name, opt_value, "x2", length))
    x2_ = length;
  else if (svg_.coordOption    (opt_name, opt_value, "y2", length))
    y2_ = length;
  else if (svg_.transformOption(opt_name, opt_value, "transform", transform))
    setTransform(transform);
  else
    return false;

  return true;
}

bool
CSVGLine::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  //---

  std::vector<CPoint2D> points;
  std::vector<double>   angles;

  double w = 1;
  double h = 1;

  double x1 = getX1().pxValue(w);
  double y1 = getY1().pxValue(h);
  double x2 = getX2().pxValue(w);
  double y2 = getY2().pxValue(h);

  points.push_back(CPoint2D(x1, y1));
  points.push_back(CPoint2D(x2, y2));

  double a = std::atan2(points[1].y - points[0].y, points[1].x - points[0].x);

  angles.push_back(a);
  angles.push_back(a); // dup

  //---

  auto *buffer = svg_.getCurrentBuffer();

  buffer->pathInit();

  buffer->pathMoveTo(points[0].x, points[0].y);
  buffer->pathLineTo(points[1].x, points[1].y);

  //---

  if (svg_.isCheckViewBox()) {
    auto dbbox = getDrawBBox();
    auto lbbox = CBBox2D(points[0], points[1]);

    if (! lbbox.overlaps(dbbox))
      CSVGLog() << "Outside viewbox : " << *this;
  }

  //---

  if (svg_.isFilled() || svg_.isStroked()) {
    if (svg_.isFilled()) {
      svg_.setFillBuffer(buffer);

      buffer->pathFill();
    }

    if (svg_.isStroked()) {
      svg_.setStrokeBuffer(buffer);

      buffer->pathStroke();
    }
  }
  else {
    svg_.setFillBuffer(buffer);

    buffer->pathFill();
  }

  //---

  svg_.drawMarkers(points, angles);

  return true;
}

bool
CSVGLine::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox())
    bbox = CBBox2D(getStart(), getEnd());
  else
    bbox = getViewBox();

  return true;
}

bool
CSVGLine::
inside(const CPoint2D &pos) const
{
  CMatrixStack2D m = getFlatTransform();

  CPoint2D p1, p2;

  m.multiplyPoint(getStart(), p1);
  m.multiplyPoint(getEnd  (), p2);

  double d = 0.0;

  if (! CMathGeom2D::PointLineDistance(pos, CLine2D(p1, p2), &d))
    return false;

  return (d <= getFlatStrokeWidth().value_or(Width(0.0)).getValue());
}

void
CSVGLine::
moveDelta(const CVector2D &delta)
{
  setStart(getStart() + delta);
  setEnd  (getEnd  () + delta);
}

void
CSVGLine::
resizeTo(const CSize2D &size)
{
  double w = 1;
  double h = 1;

  double x1 = getX1().pxValue(w);
  double y1 = getY1().pxValue(h);

  setX2(x1 + size.getWidth ());
  setY2(y1 + size.getHeight());
}

void
CSVGLine::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<line";

    printValues(os);

    os << "/>\n";
  }
  else
    os << "line " << getStart() << " " << getEnd();
}

void
CSVGLine::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameLength(os, "x1", x1_);
  printNameLength(os, "y1", y1_);
  printNameLength(os, "x2", x2_);
  printNameLength(os, "y2", y2_);
}
