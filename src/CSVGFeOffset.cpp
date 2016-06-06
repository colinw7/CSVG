#include <CSVGFeOffset.h>
#include <CSVGBuffer.h>
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

  CSVGBuffer::offsetBuffers(inBuffer, getDX(), getDY(), outBuffer);

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
