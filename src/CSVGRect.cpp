#include <CSVGRect.h>
#include <CSVGBuffer.h>
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
    x <Coordinate>
    y <Coordinate>
    width <Length>
    height <Length>
    rx <Length>
    ry <Length>
    transform <TransformList>
*/

CSVGRect::
CSVGRect(CSVG &svg) :
 CSVGObject(svg)
{
  init();
}

CSVGRect::
CSVGRect(const CSVGRect &rect) :
 CSVGObject(rect),
 x_        (rect.x_),
 y_        (rect.y_),
 width_    (rect.width_),
 height_   (rect.height_),
 rx_       (rect.rx_),
 ry_       (rect.ry_),
 bbox_     (rect.bbox_)
{
  init();
}

void
CSVGRect::
init()
{
  skipNames_.insert("x");
  skipNames_.insert("y");
  skipNames_.insert("width");
  skipNames_.insert("height");
  skipNames_.insert("rx");
  skipNames_.insert("ry");

  updateBBox();
}

CSVGRect *
CSVGRect::
dup() const
{
  return new CSVGRect(*this);
}

bool
CSVGRect::
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

  if      (svg_.coordOption (opt_name, opt_value, "x", length))
    x_ = length;
  else if (svg_.coordOption (opt_name, opt_value, "y", length))
    y_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "rx", length))
    rx_ = length.px().value();
  else if (svg_.lengthOption(opt_name, opt_value, "ry", length))
    ry_ = length.px().value();
  else if (svg_.transformOption(opt_name, opt_value, "transform", transform))
    setTransform(transform);
  else
    return false;

  updateBBox();

  return true;
}

void
CSVGRect::
updateBBox()
{
  double lw = getFlatStrokeWidth().getValue(Width(1)).getValue(1);

  auto drawBox = getDrawBBox();

  double dw = drawBox.getWidth ();
  double dh = drawBox.getHeight();

  double x = getX     ().pxValue(dw);
  double y = getY     ().pxValue(dh);
  double w = getWidth ().pxValue(dw);
  double h = getHeight().pxValue(dh);

  CPoint2D p1(x     - lw/2.0, y     - lw/2.0);
  CPoint2D p2(x + w + lw/2.0, y + h + lw/2.0);

  bbox_ = CBBox2D(p1, p2);

  parts_ = CSVGPathPartList();
}

void
CSVGRect::
strokeChanged()
{
  updateBBox();
}

bool
CSVGRect::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  auto drawBox = getDrawBBox();

  double dw = drawBox.getWidth ();
  double dh = drawBox.getHeight();

  double x = getX     ().pxValue(dw);
  double y = getY     ().pxValue(dh);
  double w = getWidth ().pxValue(dw);
  double h = getHeight().pxValue(dh);

  CPoint2D p1(x    , y    );
  CPoint2D p2(x + w, y + h);

  CBBox2D bbox(p1, p2);

  // skip zero width/height
  if (bbox.getWidth() <= 0 || bbox.getHeight() <= 0)
    return false;

  //---

  if (svg_.isCheckViewBox()) {
    CBBox2D fbbox;

    getFlatTransformedBBox(fbbox);

    if (! fbbox.overlaps(drawBox))
      CSVGLog() << "Outside viewbox : " << *this;
  }

  //---

  auto *buffer = svg_.getCurrentBuffer();

  // handle rounded rect of rx or ry specified
  if (hasRX() || hasRY()) {
    // get rx, ry (use set value for unset)
    double rx = getRX();
    double ry = getRY();

    if (! hasRX()) rx = ry;
    if (! hasRY()) ry = rx;

    // clamp to valid range
    rx = std::min(std::max(rx, 0.0), w/2.0);
    ry = std::min(std::max(ry, 0.0), h/2.0);

    // fill and stroke
    if (svg_.isFilled() || svg_.isStroked()) {
      if (svg_.isFilled())
        buffer->fillRoundedRectangle(bbox, rx, ry);

      if (svg_.isStroked())
        buffer->drawRoundedRectangle(bbox, rx, ry);
    }
    else {
      //std::cerr << "rectangle not filled or stroked" << std::endl;
      //buffer->fillRoundedRectangle(bbox, rx, ry);
    }
  }
  else {
    // fill and stroke
    if (svg_.isFilled() || svg_.isStroked()) {
      if (svg_.isFilled())
        buffer->fillRectangle(bbox);

      if (svg_.isStroked())
        buffer->drawRectangle(bbox);
    }
    else {
      //std::cerr << "rectangle not filled or stroked" << std::endl;
      //buffer->fillRectangle(bbox);
    }
  }

  return true;
}

bool
CSVGRect::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox())
    bbox = bbox_;
  else
    bbox = getViewBox();

  return true;
}

const CSVGPathPartList &
CSVGRect::
getPartList() const
{
  if (parts_.empty()) {
    // TODO: rounded rect
    auto drawBox = getDrawBBox();

    auto   dc = drawBox.getLL    ();
    double dw = drawBox.getWidth ();
    double dh = drawBox.getHeight();

    double x = getX     ().pxValue(dc.x);
    double y = getY     ().pxValue(dc.y);
    double w = getWidth ().pxValue(dw);
    double h = getHeight().pxValue(dh);

    parts_.push_back(svg_.createPathMoveTo(x, y));
    parts_.push_back(svg_.createPathHLineTo(w));
    parts_.push_back(svg_.createPathVLineTo(h));
    parts_.push_back(svg_.createPathHLineTo(-w));
    parts_.push_back(svg_.createPathClosePath());
  }

  return parts_;
}

void
CSVGRect::
setOrigin(const CPoint2D &point)
{
  x_ = point.x;
  y_ = point.y;

  updateBBox();
}

void
CSVGRect::
setSize(const CSize2D &size)
{
  width_  = size.getWidth ();
  height_ = size.getHeight();

  updateBBox();
}

void
CSVGRect::
moveTo(const CPoint2D &p)
{
  x_ = p.x;
  y_ = p.y;

  updateBBox();
}

void
CSVGRect::
moveBy(const CVector2D &delta)
{
  auto drawBox = getDrawBBox();

  double dw = drawBox.getWidth ();
  double dh = drawBox.getHeight();

  double x = getX().pxValue(dw);
  double y = getY().pxValue(dh);

  x_ = x + delta.x();
  y_ = y + delta.y();

  updateBBox();
}

void
CSVGRect::
resizeTo(const CSize2D &size)
{
  width_  = size.getWidth ();
  height_ = size.getHeight();

  updateBBox();
}

void
CSVGRect::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<rect";

    printValues(os);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</rect>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "rect (" << getId() << ") bbox " << bbox_;
}

void
CSVGRect::
printValues(std::ostream &os, bool flat) const
{
  printNameValue(os, "x", x_);
  printNameValue(os, "y", y_);

  printNameValue(os, "width" , width_ );
  printNameValue(os, "height", height_);

  printNameValue(os, "rx", rx_);
  printNameValue(os, "ry", ry_);

  CSVGObject::printValues(os, flat);
}
