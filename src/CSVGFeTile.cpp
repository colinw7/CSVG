#include <CSVGFeTile.h>
#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVGUtil.h>
#include <CSVG.h>

CSVGFeTile::
CSVGFeTile(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeTile::
CSVGFeTile(const CSVGFeTile &fe) :
 CSVGFilterBase(fe),
 filterIn_ (fe.filterIn_),
 filterOut_(fe.filterOut_)
{
}

CSVGFeTile *
CSVGFeTile::
dup() const
{
  return new CSVGFeTile(*this);
}

std::string
CSVGFeTile::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeTile::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
}

bool
CSVGFeTile::
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
CSVGFeTile::
drawElement()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);

    buffer->setBBox(inBuffer->bbox());
  }

  //---

  // get filtered object coords for input
  CBBox2D inBBox;

  getBufferSubRegion(inBuffer, inBBox);

  // get tile subregion for output
  CBBox2D outBBox;

  getSubRegion(outBBox);

  //---

  // tile
  CSVGBuffer::tileBuffers(inBuffer, inBBox, outBBox, outBuffer);

  outBuffer->setBBox(outBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);

    buffer->setBBox(outBBox);
  }

  return true;
}

void
CSVGFeTile::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feTile";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "in"    , filterIn_ );
    printNameValue(os, "result", filterOut_);

    os << "/>" << std::endl;
  }
  else
    os << "feTile ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeTile &fe)
{
  fe.print(os, false);

  return os;
}
