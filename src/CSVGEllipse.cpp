#include <CSVGEllipse.h>
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
    <Clip>
    <Mask>
    <Filter>
    <GraphicalEvents>
    <Cursor>
    <External>
    cx        <Coordinate>
    cy        <Coordinate>
    rx        <Length>
    ry        <Length>
    transform <TransformList>
*/

CSVGEllipse::
CSVGEllipse(CSVG &svg) :
 CSVGObject(svg)
{
  init();
}

CSVGEllipse::
CSVGEllipse(const CSVGEllipse &ellipse) :
 CSVGObject(ellipse),
 cx_       (ellipse.cx_),
 cy_       (ellipse.cy_),
 rx_       (ellipse.rx_),
 ry_       (ellipse.ry_)
{
  init();
}

void
CSVGEllipse::
init()
{
  skipNames_.insert("cx");
  skipNames_.insert("cy");
  skipNames_.insert("rx");
  skipNames_.insert("ry");

  updateBBox();
}

CSVGEllipse *
CSVGEllipse::
dup() const
{
  return new CSVGEllipse(*this);
}

CPoint2D
CSVGEllipse::
getCenter() const
{
  double xc = getCenterX().pxValue(CScreenUnits(1));
  double yc = getCenterY().pxValue(CScreenUnits(1));

  return CPoint2D(xc, yc);
}

void
CSVGEllipse::
setCenter(const CPoint2D &center)
{
  setCenterX(CScreenUnits(center.x), /*update*/false);
  setCenterY(CScreenUnits(center.y));
}

bool
CSVGEllipse::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processPaintOption          (opt_name, opt_value)) return true;
  if (processColorOption          (opt_name, opt_value)) return true;
  if (processOpacityOption        (opt_name, opt_value)) return true;
  if (processGraphicsOption       (opt_name, opt_value)) return true;
  if (processClipOption           (opt_name, opt_value)) return true;
  if (processMaskOption           (opt_name, opt_value)) return true;
  if (processFilterOption         (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processCursorOption         (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  std::string    str;
  CScreenUnits   length;
  CMatrixStack2D transform;

  if      (svg_.coordOption    (opt_name, opt_value, "cx", length))
    cx_ = length;
  else if (svg_.coordOption    (opt_name, opt_value, "cy", length))
    cy_ = length;
  else if (svg_.lengthOption   (opt_name, opt_value, "rx" , length))
    rx_ = length;
  else if (svg_.lengthOption   (opt_name, opt_value, "ry" , length))
    ry_ = length;
  else if (svg_.transformOption(opt_name, opt_value, "transform", transform))
    setTransform(transform);
  else
    return false;

  updateBBox();

  return true;
}

void
CSVGEllipse::
updateBBox()
{
  // stroke width ?
  double xc = getCenterX().pxValue(CScreenUnits(1));
  double yc = getCenterY().pxValue(CScreenUnits(1));
  double xr = getRadiusX().pxValue(CScreenUnits(1));
  double yr = getRadiusY().pxValue(CScreenUnits(1));

  bbox_ = CBBox2D(xc - xr, yc - yr, xc + xr, yc + yr);

  parts_ = CSVGPathPartList();
}

bool
CSVGEllipse::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  auto drawBox = getDrawBBox();

  double dw = drawBox.getWidth ();
  double dh = drawBox.getHeight();

  double xc = getCenterX().pxValue(CScreenUnits(dw));
  double yc = getCenterY().pxValue(CScreenUnits(dh));
  double xr = getRadiusX().pxValue(CScreenUnits(dw));
  double yr = getRadiusY().pxValue(CScreenUnits(dh));

  // skip zero radius
  if (xr <= 0 || yr <= 0)
    return false;

  //---

  if (svg_.isCheckViewBox()) {
    CBBox2D fbbox;

    getFlatTransformedBBox(fbbox);

    if (! fbbox.overlaps(drawBox))
      CSVGLog() << "Outside viewbox : " << *this;
  }

  //---

  svg_.drawEllipse(xc, yc, xr, yr);

  return true;
}

bool
CSVGEllipse::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox())
    bbox = bbox_;
  else
    bbox = getViewBox();

  return true;
}

const CSVGPathPartList &
CSVGEllipse::
getPartList() const
{
  if (parts_.empty()) {
    auto drawBox = getDrawBBox();

    auto dc = drawBox.getCenter();
    auto dw = drawBox.getWidth ();
    auto dh = drawBox.getHeight();

    double x  = getCenterX().pxValue(dc.x);
    double y  = getCenterY().pxValue(dc.y);
    double rx = getRadiusX().pxValue(dw/2.0);
    double ry = getRadiusY().pxValue(dh/2.0);

    double fa = 1;
    double fs = 0;

    parts_.push_back(svg_.createPathMoveTo(x + rx, y));
    parts_.push_back(svg_.createPathArcTo (rx, ry, M_PI, fa, fs, x - rx, y));
    parts_.push_back(svg_.createPathArcTo (rx, ry, M_PI, fa, fs, x + rx, y));
    parts_.push_back(svg_.createPathClosePath());
  }

  return parts_;
}

void
CSVGEllipse::
moveDelta(const CVector2D &delta)
{
  auto c = getCenter();

  c += delta;

  setCenter(c);
}

void
CSVGEllipse::
resizeTo(const CSize2D &size)
{
  setRadius(size.getWidth()/2.0, size.getHeight()/2.0);
}

void
CSVGEllipse::
print(std::ostream &os, bool hier) const
{
  if (hier)
    CSVGObject::print(os, hier);
  else
    os << "ellipse (" << getId() << ") center " <<
          getCenter() << " radius " << getRadiusX() << " " << getRadiusY();
}

void
CSVGEllipse::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue(os, "cx", cx_);
  printNameValue(os, "cy", cy_);
  printNameValue(os, "rx", rx_);
  printNameValue(os, "ry", ry_);
}
