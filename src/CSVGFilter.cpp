#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVGUtil.h>
#include <CSVG.h>

CSVGFilter::
CSVGFilter(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGFilter::
CSVGFilter(const CSVGFilter &filter) :
 CSVGObject(filter),
 xlink_    (this)
{
}

CSVGFilter *
CSVGFilter::
dup() const
{
  return new CSVGFilter(*this);
}

void
CSVGFilter::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

bool
CSVGFilter::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string    str;
  CSVGCoordUnits units;
  CScreenUnits   length;

  if      (svg_.coordUnitsOption(opt_name, opt_value, "filterUnits", units))
    filterUnits_ = units;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "primitiveUnits", units))
    primitiveUnits_ = units;
  else if (svg_.coordOption(opt_name, opt_value, "x", length))
    x_ = length;
  else if (svg_.coordOption(opt_name, opt_value, "y", length))
    y_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length;
  else if (svg_.stringOption(opt_name, opt_value, "filterRes", str))
    filterRes_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else if (processFilterColorOption(opt_name, opt_value))
    return true;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFilter::
getRegion(CBBox2D &bbox) const
{
  return getRegion(getObject(), bbox);
}

CBBox2D
CSVGFilter::
getObjectBBox() const
{
  return getObjectBBox(getObject());
}

CBBox2D
CSVGFilter::
getObjectBBox(CSVGObject *obj) const
{
  // get reference parent bbox
  CBBox2D bbox;

  //if (! obj->getBBox(bbox1)) {
  if (! obj->getFlatTransformedBBox(bbox)) {
    if (! obj->getParentViewBox(bbox))
      bbox = CBBox2D(0, 0, 100, 100);
  }

  assert(bbox.isSet());

  return bbox;
}

bool
CSVGFilter::
getRegion(CSVGObject *obj, CBBox2D &bbox) const
{
  CBBox2D bbox1 = getObjectBBox(obj);

  //---

  // get filter specified x, y, width, height in filterUnits units
  // default is (x=-0.1, y=-0.1, w=1.2, h=1.2)
  CSVGCoordUnits filterUnits = getFilterUnits();

  double x = 0, y = 0;

  if (hasX()) {
    double x1 = getX().pxValue(1);

    if (filterUnits == CSVGCoordUnits::OBJECT_BBOX)
      x = CSVGUtil::map(x1, 0, 1, bbox1.getXMin(), bbox1.getXMax());
    else
      x = x1;
  }
  else
    x = CSVGUtil::map(-0.1, 0, 1, bbox1.getXMin(), bbox1.getXMax());

  if (hasY()) {
    double y1 = getY().pxValue(1);

    if (filterUnits == CSVGCoordUnits::OBJECT_BBOX)
      y = CSVGUtil::map(y1, 0, 1, bbox1.getYMin(), bbox1.getYMax());
    else
      y = y1;
  }
  else
    y = CSVGUtil::map(-0.1, 0, 1, bbox1.getYMin(), bbox1.getYMax());

  //---

  double w, h;

  if (hasWidth()) {
    w = getWidth().pxValue(1);

    if (filterUnits == CSVGCoordUnits::OBJECT_BBOX)
      w *= bbox1.getWidth();
  }
  else
    w = 1.2*bbox1.getWidth();

  if (hasHeight()) {
    h = getHeight().pxValue(1);

    if (filterUnits == CSVGCoordUnits::OBJECT_BBOX)
      h *= bbox1.getHeight();
  }
  else
    h = 1.2*bbox1.getHeight();

  // TODO: negtive width or height is an error
  // TODO: zero width or height then rendering of parent object is disabled

  //---

  bbox = CBBox2D(x, y, x + w, y + h);

  return true;
}

void
CSVGFilter::
initDraw(CSVGBuffer *buffer)
{
  lastElement_ = nullptr;

  offset_ = CPoint2D();

  // use update background image
  if (buffer->parentBuffer()) {
    CSVGBuffer *bgBuffer  = svg_.getBuffer("BackgroundImage");
    CSVGBuffer *bgBuffer1 = svg_.getBuffer(buffer->getName() + "_FilterBackgroundImage1");
    CSVGBuffer *bgBuffer2 = svg_.getBuffer(buffer->getName() + "_FilterBackgroundImage2");

    bool bg_drawing = bgBuffer->isDrawing();

    if (bg_drawing)
      bgBuffer->stopDraw();

    bgBuffer1->setImageBuffer(bgBuffer);

    bool parent_drawing = buffer->parentBuffer()->isDrawing();

    if (parent_drawing)
      buffer->parentBuffer()->stopDraw();

    bgBuffer2->setFlatImageBuffer(buffer->parentBuffer());

    bgBuffer->setImageBuffer(bgBuffer2);

    if (bg_drawing)
      bgBuffer->startDraw();

    if (parent_drawing)
      buffer->parentBuffer()->startDraw();
  }

  //---

  // init last filter name
  lastFilterName_ = "FilterGraphic";

  // store current buffer image into SourceGraphic and in filter
  CSVGBuffer *srcBuffer = svg_.getBuffer("SourceGraphic");
  CSVGBuffer *fltBuffer = svg_.getBuffer(lastFilterName_);

  // reset input bboxes
  srcBuffer->setBBox(CBBox2D());
  fltBuffer->setBBox(CBBox2D());

  //---

  CBBox2D bbox;

  getRegion(bbox);

  svg_.setPaintBox(bbox);

  if (object_) {
    CSVGFill   fill   = object_->getFlatFill  ();
    CSVGStroke stroke = object_->getFlatStroke();

    svg_.setPaintStyle(fill, stroke);
  }

  //---

  //bool srcDrawing = srcBuffer->isDrawing();
  // stop drawing to SourceGraphic if needed
  //if (srcDrawing) srcBuffer->stopDraw();

  oldDrawing_ = buffer->isDrawing();

  if (oldDrawing_)
    buffer->stopDraw();

  //srcBuffer->setFlatImageBuffer(buffer);
  srcBuffer->setImageBuffer(buffer);
  fltBuffer->setImageBuffer(srcBuffer);

  // save current image into filter in
  if (svg_.getDebugFilter()) {
    CSVGBuffer *filterInBuffer = svg_.getBuffer(buffer->getName() + "_filter_in");

    filterInBuffer->setImageBuffer(srcBuffer);
    filterInBuffer->setBBox       (bbox);
  }
}

void
CSVGFilter::
termDraw(CSVGBuffer *buffer)
{
  // restore background
  if (buffer->parentBuffer()) {
    CSVGBuffer *bgBuffer  = svg_.getBuffer("BackgroundImage");
    CSVGBuffer *bgBuffer1 = svg_.getBuffer(buffer->getName() + "_FilterBackgroundImage1");

    bool bg_drawing = bgBuffer->isDrawing();

    if (bg_drawing)
      bgBuffer->stopDraw();

    bgBuffer->setImageBuffer(bgBuffer1);

    if (bg_drawing)
      bgBuffer->startDraw();
  }

  //---

  CSVGBuffer *fltBuffer = svg_.getBuffer(lastFilterName_);

  if (svg_.getDebugFilter()) {
    // save last filter output name (output of filter) into filter_out
    CSVGBuffer *filterOutBuffer = svg_.getBuffer(buffer->getName() + "_filter_out");

    filterOutBuffer->setImageBuffer(fltBuffer);
    filterOutBuffer->setBBox       (buffer->bbox());
  }

  // store filter image back into current buffer
  buffer->setImageBuffer(fltBuffer);

  // restart drawing to SourceGraphic if needed
  //if (srcDrawing)
  //  fltBuffer->startDraw();

  if (oldDrawing_)
    buffer->startDraw();
}

bool
CSVGFilter::
draw()
{
  return false;
}

void
CSVGFilter::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<filter";

    printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</filter>" << std::endl;
  }
  else
    os << "filter ";
}

void
CSVGFilter::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameCoordUnits(os, "filterUnits"   , filterUnits_);
  printNameCoordUnits(os, "primitiveUnits", primitiveUnits_);

  printNameLength(os, "x"     , x_);
  printNameLength(os, "y"     , y_);
  printNameLength(os, "width" , width_);
  printNameLength(os, "height", height_);

  printNameValue(os, "filterRes" , filterRes_);

  if (! xlink_.getValue().isNull())
    printNameXLink(os, "xlink:href", xlink_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFilter &filter)
{
  filter.print(os, false);

  return os;
}
