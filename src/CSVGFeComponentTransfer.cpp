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
draw()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
  }

  std::vector<CSVGFeFunc *> funcs;

  for (const auto &c : children()) {
    CSVGFeFunc *func = dynamic_cast<CSVGFeFunc *>(c);
    if (! func) continue;

    funcs.push_back(func);
  }

  CSVGBuffer::componentTransferBuffers(inBuffer, funcs, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
  }

  return true;
}

void
CSVGFeComponentTransfer::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feComponentTransfer";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "in"    , filterIn_ );
    printNameValue(os, "result", filterOut_);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</feComponentTransfer>" << std::endl;
  }
  else
    os << "feComponentTransfer ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeComponentTransfer &fe)
{
  fe.print(os, false);

  return os;
}
