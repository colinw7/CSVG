#include <CSVGFeComponentTransfer.h>
#include <CSVGFeFunc.h>
#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeComponentTransfer::
CSVGFeComponentTransfer(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeComponentTransfer::
CSVGFeComponentTransfer(const CSVGFeComponentTransfer &fe) :
 CSVGFilterBase(fe),
 filterIn_ (fe.filterIn_),
 filterOut_(fe.filterOut_)
{
}

CSVGFeComponentTransfer *
CSVGFeComponentTransfer::
dup() const
{
  return new CSVGFeComponentTransfer(*this);
}

std::string
CSVGFeComponentTransfer::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeComponentTransfer::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
}

bool
CSVGFeComponentTransfer::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeComponentTransfer::
drawElement()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing = inBuffer->isDrawing();

  if (inDrawing) inBuffer->stopDraw();

  //---

  // get filtered object coords
  CBBox2D inBBox;

  getBufferSubRegion(inBuffer, inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  std::vector<CSVGFeFunc *> funcs;

  for (const auto &c : children()) {
    auto *func = dynamic_cast<CSVGFeFunc *>(c);
    if (! func) continue;

    funcs.push_back(func);
  }

  CSVGBuffer::componentTransferBuffers(inBuffer, inBBox, funcs, outBuffer);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  if (inDrawing) inBuffer->startDraw();

  return true;
}

void
CSVGFeComponentTransfer::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feComponentTransfer";

    printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</feComponentTransfer>" << std::endl;
  }
  else
    os << "feComponentTransfer ";
}

void
CSVGFeComponentTransfer::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue(os, "in"    , filterIn_ );
  printNameValue(os, "result", filterOut_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeComponentTransfer &fe)
{
  fe.print(os, false);

  return os;
}
