#include <CSVGFeGaussianBlur.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeGaussianBlur::
CSVGFeGaussianBlur(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeGaussianBlur::
CSVGFeGaussianBlur(const CSVGFeGaussianBlur &blur) :
 CSVGFilterBase(blur),
 filterIn_ (blur.filterIn_),
 filterOut_(blur.filterOut_),
 stdDev_   (blur.stdDev_)
{
}

CSVGFeGaussianBlur *
CSVGFeGaussianBlur::
dup() const
{
  return new CSVGFeGaussianBlur(*this);
}

bool
CSVGFeGaussianBlur::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.realOption  (opt_name, opt_value, "stdDeviation", &real))
    stdDev_ = real;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeGaussianBlur::
draw()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImage(inBuffer->getImage());
  }

  filterImage(inBuffer, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
  }
}

void
CSVGFeGaussianBlur::
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
{
  CSVGBuffer::gaussianBlurBuffers(inBuffer, getStdDev(), outBuffer);
}

void
CSVGFeGaussianBlur::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feGaussianBlur";

    CSVGObject::printValues(os);

    printNameValue(os, "in"          , filterIn_ );
    printNameValue(os, "result"      , filterOut_);
    printNameValue(os, "stdDeviation", stdDev_   );

    os << "/>" << std::endl;
  }
  else
    os << "feGaussianBlur ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeGaussianBlur &filter)
{
  filter.print(os, false);

  return os;
}
