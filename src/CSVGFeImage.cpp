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
draw()
{
  // get object or file image into buffer (untransformed)
  CSVGBuffer *inBuffer = svg_.getBuffer(getUniqueName() + "_xlink");

  CMatrixStack2D transform = svg_.getCurrentBuffer()->transform();

  if (hasLink()) {
    if (! xlink().getImage(inBuffer))
      return false;
  }
  else
    return false;

  //---

  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
  }

  // put image into output buffer (transformed)
  CSVGBuffer::imageBuffers(inBuffer, transform, getPreserveAspect(), outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);

    buffer->setBBox(outBuffer->bbox());
  }

  return true;
}

void
CSVGFeImage::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feImage";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue         (os, "filter_out"         , filterOut_);
    printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);

    os << "/>" << std::endl;
  }
  else
    os << "feImage ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeImage &filter)
{
  filter.print(os, false);

  return os;
}
