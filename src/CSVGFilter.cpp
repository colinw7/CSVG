#include <CSVGFilter.h>
#include <CSVGBuffer.h>
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

bool
CSVGFilter::
getRegion(CSVGObject *obj, CBBox2D &bbox) const
{
  CBBox2D bbox1;

  if (! obj->getBBox(bbox1)) {
    // TODO: viewBBox
    //if (! obj->getFlatTransformedBBox(bbox1))
    // return false;
    bbox1 = CBBox2D(0,0,100,100);
  }

  if (! bbox1.isSet())
    return false;

  double w1 = bbox1.getWidth ();
  double h1 = bbox1.getHeight();

  double x1, y1, x2, y2;

  if (hasX()) {
    double x = getX().pxValue(1);

    if (getUnits() == CSVGCoordUnits::OBJECT_BBOX)
      x1 = bbox1.getXMin() + x*w1;
    else
      x1 = x;
  }
  else
    x1 = bbox1.getXMin() - 0.1*w1;

  if (hasY()) {
    double y = getY().pxValue(1);

    if (getUnits() == CSVGCoordUnits::OBJECT_BBOX)
      y1 = bbox1.getYMin() + y*h1;
    else
      y1 = y;
  }
  else
    y1 = bbox1.getYMin() - 0.1*h1;

  if (hasWidth()) {
    double w = getWidth().pxValue(1);

    if (getUnits() == CSVGCoordUnits::OBJECT_BBOX)
      x2 = bbox1.getXMin() + w*w1;
    else
      x2 = bbox1.getXMin() + w;
  }
  else
    x2 = bbox1.getXMax() + 0.1*w1;

  if (hasHeight()) {
    double h = getHeight().pxValue(1);

    if (getUnits() == CSVGCoordUnits::OBJECT_BBOX)
      y2 = bbox1.getYMin() + h*h1;
    else
      y2 = bbox1.getYMin() + h;
  }
  else
    y2 = bbox1.getYMax() + 0.1*h1;

  bbox = CBBox2D(x1, y1, x2, y2);

  return true;
}

void
CSVGFilter::
initDraw(CSVGBuffer *buffer)
{
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

  //---

  CBBox2D bbox;

  getRegion(bbox);

  svg_.setPaintBox(bbox);

  if (object_) {
    CSVGColor fillColor   = object_->getFlatFillColor();
    CSVGColor strokeColor = object_->getFlatStrokeColor();

    svg_.setPaintColors(fillColor, strokeColor);
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

    CSVGObject::printValues(os);

    printNameCoordUnits(os, "filterUnits"   , filterUnits_);
    printNameCoordUnits(os, "primitiveUnits", primitiveUnits_);

    printNameLength(os, "x"     , x_);
    printNameLength(os, "y"     , y_);
    printNameLength(os, "width" , width_);
    printNameLength(os, "height", height_);

    printNameValue(os, "filterRes" , filterRes_);
    printNameXLink(os, "xlink:href", xlink_);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</filter>" << std::endl;
  }
  else
    os << "filter ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFilter &filter)
{
  filter.print(os, false);

  return os;
}
