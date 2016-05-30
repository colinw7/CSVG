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
    units_ = units;
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
getRegion(CSVGObject *obj, CBBox2D &bbox) const
{
  CBBox2D bbox1;

  if (! obj->getBBox(bbox1))
  //if (! obj->getFlatTransformedBBox(bbox1))
    return false;

  if (! bbox1.isSet())
    return false;

  double w = bbox1.getWidth ();
  double h = bbox1.getHeight();

  double x1, y1, x2, y2;

  if (hasX())
    x1 = getX().pxValue(w);
  else
    x1 = bbox1.getXMin() - 0.1*w;

  if (hasY())
    y1 = getY().pxValue(h);
  else
    y1 = bbox1.getYMin() - 0.1*h;

  if (hasWidth())
    x2 = x1 + getWidth().pxValue(w);
  else
    x2 = bbox1.getXMax() + 0.1*w;

  if (hasHeight())
    y2 = y1 + getHeight().pxValue(h);
  else
    y2 = bbox1.getYMax() + 0.1*h;

  bbox = CBBox2D(x1, y1, x2, y2);

  return true;
}

void
CSVGFilter::
initDraw(CSVGBuffer *buffer)
{
  // store current buffer image into SourceGraphic and FilterGraphic
  CSVGBuffer *srcBuffer = svg_.getBuffer("SourceGraphic");
  CSVGBuffer *fltBuffer = svg_.getBuffer("FilterGraphic");

  //bool srcDrawing = srcBuffer->isDrawing();
  // stop drawing to SourceGraphic if needed
  //if (srcDrawing) srcBuffer->stopDraw();

  oldDrawing_ = buffer->isDrawing();

  if (oldDrawing_)
    buffer->stopDraw();

  srcBuffer->setImage(buffer);
  fltBuffer->setImage(buffer);

  // save current image into filter in
  if (svg_.getDebugFilter()) {
    CSVGBuffer *filterInBuffer = svg_.getBuffer(buffer->getName() + "_filter_in");

    filterInBuffer->setImage(srcBuffer);
  }
}

void
CSVGFilter::
termDraw(CSVGBuffer *buffer)
{
  CSVGBuffer *fltBuffer = svg_.getBuffer("FilterGraphic");

  if (svg_.getDebugFilter()) {
    // save FilterGraphic (output of filter) into filter_out
    CSVGBuffer *filterOutBuffer = svg_.getBuffer(buffer->getName() + "_filter_out");

    filterOutBuffer->setImage(fltBuffer);
  }

  // store filter image back into current buffer
  buffer->setImage(fltBuffer);

  // restart drawing to SourceGraphic if needed
  //if (srcDrawing)
  //  fltBuffer->startDraw();

  if (oldDrawing_)
    buffer->startDraw();
}

void
CSVGFilter::
draw()
{
}

void
CSVGFilter::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<filter";

    CSVGObject::printValues(os);

    printNameCoordUnits(os, "filterUnits"   , units_);
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
