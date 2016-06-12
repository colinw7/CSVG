#include <CSVGFeOffset.h>
#include <CSVGBuffer.h>
#include <CSVGFilter.h>
#include <CSVG.h>

CSVGFeOffset::
CSVGFeOffset(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeOffset::
CSVGFeOffset(const CSVGFeOffset &offset) :
 CSVGFilterBase(offset),
 filterIn_ (offset.filterIn_),
 filterOut_(offset.filterOut_),
 dx_       (offset.dx_),
 dy_       (offset.dy_)
{
}

CSVGFeOffset *
CSVGFeOffset::
dup() const
{
  return new CSVGFeOffset(*this);
}

std::string
CSVGFeOffset::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeOffset::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
}

bool
CSVGFeOffset::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.realOption  (opt_name, opt_value, "dx", &real))
    dx_ = real;
  else if (svg_.realOption  (opt_name, opt_value, "dy", &real))
    dy_ = real;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeOffset::
draw()
{
  std::string objectBufferName = "_" + getUniqueName();

  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing = inBuffer->isDrawing();

  if (inDrawing)
    inBuffer->stopDraw();

  if (svg_.getDebugFilter()) {
    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
  }

  //---

  // get filtered object coords
  CSVGFilter *filter = getParentFilter();

  CBBox2D filterBBox;

  if (filter)
    filter->getRegion(filterBBox);

  CBBox2D filterBaseBBox;

  getTransformedParentBBox(filterBaseBBox);

  if (! filterBBox.isSet())
    filterBBox = filterBaseBBox;

  if (! filterBaseBBox.isSet())
    filterBaseBBox = filterBBox;

  double x1 = 0, y1 = 0, x2 = -1, y2 = -1;

  if (filterBaseBBox.isSet()) {
    svg_.windowToPixel(filterBaseBBox.getXMin(), filterBaseBBox.getYMin(), &x1, &y1);
    svg_.windowToPixel(filterBaseBBox.getXMax(), filterBaseBBox.getYMax(), &x2, &y2);
  }

  //---

  double dx = getDX();
  double dy = getDY();

  CSVGBuffer::offsetBuffers(inBuffer, x1, y1, x2 - x1, y2 - y1, dx, dy, outBuffer);

  if (filterBaseBBox.isSet())
    outBuffer->setBBox(filterBaseBBox);

  //---

  if (svg_.getDebugFilter()) {
    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
  }

  if (inDrawing)
    inBuffer->startDraw();

  return true;
}

void
CSVGFeOffset::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feOffset";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "in"    , filterIn_ );
    printNameValue(os, "result", filterOut_);
    printNameValue(os, "dx"    , dx_       );
    printNameValue(os, "dy"    , dy_       );

    os << "/>" << std::endl;
  }
  else
    os << "feOffset ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeOffset &filter)
{
  filter.print(os, false);

  return os;
}
