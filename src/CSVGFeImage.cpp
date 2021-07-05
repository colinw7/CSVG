#include <CSVGFeImage.h>
#include <CSVGFilter.h>
#include <CSVG.h>
#include <CSVGBuffer.h>

CSVGFeImage::
CSVGFeImage(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeImage::
CSVGFeImage(const CSVGFeImage &image) :
 CSVGFilterBase(image),
 filterOut_(image.filterOut_)
{
}

CSVGFeImage *
CSVGFeImage::
dup() const
{
  return new CSVGFeImage(*this);
}

std::string
CSVGFeImage::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
}

bool
CSVGFeImage::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string        str;
  CSVGPreserveAspect preserveAspect;

  if      (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.preserveAspectOption(opt_name, opt_value, "preserveAspectRatio", preserveAspect))
    preserveAspect_ = preserveAspect;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeImage::
drawElement()
{
  // get object or file image into buffer (untransformed)
  auto *inBuffer = svg_.getBuffer(getUniqueName() + "_xlink");

  if (hasLink()) {
    if (! xlink().getImage(inBuffer))
      return false;
  }
  else
    return false;

  //---

  CBBox2D bbox;

  getSubRegion(bbox);

  //---

  auto *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
  }

  //---

  // put image into output buffer
  CSVGBuffer::imageBuffers(inBuffer, bbox, getPreserveAspect(), outBuffer);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (outBuffer->bbox());
  }

  return true;
}

void
CSVGFeImage::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feImage";

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "feImage ";
}

void
CSVGFeImage::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue         (os, "result"             , filterOut_);
  printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeImage &filter)
{
  filter.print(os, false);

  return os;
}
