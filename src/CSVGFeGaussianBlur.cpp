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
 stdDevX_  (blur.stdDevX_),
 stdDevY_  (blur.stdDevY_)
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
  std::string         str;
  std::vector<double> reals;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.realListOption(opt_name, opt_value, "stdDeviation", reals)) {
    if      (reals.size() == 1)
      stdDevX_ = reals[0];
    else if (reals.size() > 1) {
      stdDevX_ = reals[0];
      stdDevY_ = reals[1];
    }
  }
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeGaussianBlur::
draw()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing = inBuffer->isDrawing();

  if (inDrawing)
    inBuffer->stopDraw();

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
  }

  filterImage(inBuffer, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
  }

  if (inDrawing)
    inBuffer->startDraw();

  return true;
}

void
CSVGFeGaussianBlur::
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
{
  CSVGBuffer::gaussianBlurBuffers(inBuffer, this, getStdDevX(), getStdDevY(), outBuffer);
}

void
CSVGFeGaussianBlur::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feGaussianBlur";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "in"          , filterIn_ );
    printNameValue(os, "result"      , filterOut_);

    if      (stdDevX_.isValid() && ! stdDevY_.isValid())
      printNameValue(os, "stdDeviation", stdDevX_);
    else if (stdDevX_.isValid() && stdDevY_.isValid())
      os << " stdDeviation=\"" << stdDevX_.getValue() << " " << stdDevY_.getValue() << "\"";

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
