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
getSubRegion(CBBox2D &bbox) const
{
  bool rc = CSVGFilterBase::getSubRegion(bbox);

  if (! drawing_) {
    double dx, dy;

    calcDelta(dx, dy);

    bbox.moveBy(CPoint2D(dx, dy));
  }

  return rc;
}

bool
CSVGFeOffset::
drawElement()
{
  std::string objectBufferName = "_" + getUniqueName();

  auto *inBuffer  = svg_.getBuffer(getFilterIn ());
  auto *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing = inBuffer->isDrawing();

  if (inDrawing) inBuffer->stopDraw();

  //---

  // get filtered object coords
  drawing_ = true;

  CBBox2D inBBox;

  getBufferSubRegion(inBuffer, inBBox);

  drawing_ = false;

  //---

  double dx, dy;

  calcDelta(dx, dy);

  //auto outBBox = inBBox.movedBy(CPoint2D(dx, dy));

  //---

  if (svg_.getDebugFilter()) {
    auto *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  auto *filter = getParentFilter();

  if (filter)
    filter->setOffset(CPoint2D(dx, dy));

  CSVGBuffer::offsetBuffers(inBuffer, inBBox, dx, dy, outBuffer);

  outBuffer->setBBox(inBBox);

  //---

  if (svg_.getDebugFilter()) {
    auto *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  if (inDrawing) inBuffer->startDraw();

  return true;
}

void
CSVGFeOffset::
calcDelta(double &dx, double &dy) const
{
  dx = this->getDX();
  dy = this->getDY();

  auto *filter = getParentFilter();

  auto primitiveUnits = filter->getPrimitiveUnits();

  if (primitiveUnits == CSVGCoordUnits::OBJECT_BBOX) {
    auto filterBBox = filter->getObjectBBox();

    dx *= filterBBox.getWidth ();
    dy *= filterBBox.getHeight();
  }
}

void
CSVGFeOffset::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feOffset";

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "feOffset ";
}

void
CSVGFeOffset::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue(os, "in"    , filterIn_ );
  printNameValue(os, "result", filterOut_);
  printNameValue(os, "dx"    , dx_       );
  printNameValue(os, "dy"    , dy_       );
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeOffset &filter)
{
  filter.print(os, false);

  return os;
}
