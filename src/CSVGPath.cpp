#include <CSVGPath.h>
#include <CSVGPathPart.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGPath::
CSVGPath(CSVG &svg) :
 CSVGObject(svg)
{
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
  double           r;
  CSVGPathPartList parts;

  if      (svg_.pathOption(opt_name, opt_value, "d", parts))
    parts_ = parts;
  else if (svg_.realOption(opt_name, opt_value, "pathLength", &r))
    pathLength_ = r;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGPath::
moveDelta(const CVector2D &d)
{
  parts_.moveBy(d);
}

bool
CSVGPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  bbox_.setInvalid();

  // zero length path with line cap square or round draw square of circle shape to match line width
  double l = parts_.getLength();

  if (l == 0)
    return drawZeroLength();

  //---

  svg_.drawParts(parts_);

  //---

  if (svg_.isCheckViewBox()) {
    auto drawBox = getDrawBBox();

    CBBox2D fbbox;

    getFlatTransformedBBox(fbbox);

    if (! fbbox.overlaps(drawBox))
      CSVGLog() << "Outside viewbox : " << *this;
  }

  return true;
}

bool
CSVGPath::
drawZeroLength()
{
  bool drawCap = true;

  if (! svg_.isStroked())
    drawCap = false;

  if (drawCap && ! stroke_.getLineCapValid())
    drawCap = false;

  auto   c = getFlatStrokeColor();
  double r = getFlatStrokeWidth().getValue(Width(1)).getValue()/2;

  auto *buffer = svg_.getCurrentBuffer();

  CPoint2D p;

  if (drawCap) {
    std::vector<CPoint2D> points;
    std::vector<double>   angles;

    buffer->pathInit();

    parts_.draw(buffer, points, angles);

    if (! points.empty())
      p = points[0];
    else
      drawCap = false;
  }

  if (! drawCap)
    return false;

  buffer->resetFill();

  if (c.isValid())
    buffer->setFillColor(c.getValue().getValue().rgba());

  auto lineCap = stroke_.getLineCap().getValue();

  if (lineCap != LINE_CAP_TYPE_SQUARE && lineCap != LINE_CAP_TYPE_ROUND)
    return false;

  buffer->pathInit();

  if (lineCap == LINE_CAP_TYPE_SQUARE) {
    CBBox2D bbox(p.x - r, p.y - r, p.x + r, p.y + r);

    buffer->pathMoveTo(bbox.getXMin(), bbox.getYMin());
    buffer->pathLineTo(bbox.getXMax(), bbox.getYMin());
    buffer->pathLineTo(bbox.getXMax(), bbox.getYMax());
    buffer->pathLineTo(bbox.getXMin(), bbox.getYMax());
  }
  else {
    buffer->pathMoveTo(p.x + r, p.y);
    buffer->pathArcTo (p.x, p.y, r, r, 0,  M_PI);
    buffer->pathArcTo (p.x, p.y, r, r, M_PI, 2*M_PI);
  }

  buffer->pathClose();

  buffer->pathFill();

  return true;
}

void
CSVGPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<path";

    printValues(os, /*flat*/false);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</path>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else {
    os << "path (" << getId() << ") parts (" << parts_ << ")";
  }
}

void
CSVGPath::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameParts(os, "d"         , parts_);
  printNameValue(os, "pathLength", pathLength_);
}

bool
CSVGPath::
getBBox(CBBox2D &bbox) const
{
  bool rc = true;

  if (! bbox_.isValid()) {
    if (hasViewBox())
      bbox = getViewBox();
    else {
      auto *currentBuffer = svg_.getCurrentBuffer();

      rc = parts_.getBBox(currentBuffer, bbox);
    }

    double lw = getFlatStrokeWidth().getValue(Width(1)).getValue(1);

    bbox.expand(lw);

    bbox_ = bbox;
  }
  else
    bbox = bbox_.getValue();

  return rc;
}
