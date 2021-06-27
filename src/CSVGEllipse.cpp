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
}

CSVGEllipse::
CSVGEllipse(const CSVGEllipse &ellipse) :
 CSVGObject(ellipse),
 cx_       (ellipse.cx_),
 cy_       (ellipse.cy_),
 rx_       (ellipse.rx_),
 ry_       (ellipse.ry_)
{
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
  setCenterX(CScreenUnits(center.x));
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

  return true;
}

bool
CSVGEllipse::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  auto drawBox = getDrawBBox();

  double w = drawBox.getWidth ();
  double h = drawBox.getHeight();

  double xc = getCenterX().pxValue(CScreenUnits(w));
  double yc = getCenterY().pxValue(CScreenUnits(h));
  double xr = getRadiusX().pxValue(CScreenUnits(w));
  double yr = getRadiusY().pxValue(CScreenUnits(h));

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
  if (! hasViewBox()) {
    double xc = getCenterX().pxValue(CScreenUnits(1));
    double yc = getCenterY().pxValue(CScreenUnits(1));
    double xr = getRadiusX().pxValue(CScreenUnits(1));
    double yr = getRadiusY().pxValue(CScreenUnits(1));

    bbox = CBBox2D(xc - xr, yc - yr, xc + xr, yc + yr);
  }
  else
    bbox = getViewBox();

  return true;
}

void
CSVGEllipse::
moveBy(const CVector2D &delta)
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
  if (hier) {
    CSVGObject::print(os, hier);
  }
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

std::ostream &
operator<<(std::ostream &os, const CSVGEllipse &ellipse)
{
  ellipse.print(os, false);

  return os;
}
