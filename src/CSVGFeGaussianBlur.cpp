#include <CSVGFeGaussianBlur.h>
#include <CSVGFilter.h>
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

std::string
CSVGFeGaussianBlur::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeGaussianBlur::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
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
drawElement()
{
  auto *inBuffer  = svg_.getBuffer(getFilterIn ());
  auto *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing = inBuffer->isDrawing();

  if (inDrawing) inBuffer->stopDraw();

  //---

  // get filtered object coords
  CBBox2D inBBox;

  getBufferSubRegion(inBuffer, inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  CSVGBuffer::gaussianBlurBuffers(inBuffer, inBBox, getStdDevX(), getStdDevY(), outBuffer);

  outBuffer->setBBox(inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  if (inDrawing) inBuffer->startDraw();

  return true;
}

void
CSVGFeGaussianBlur::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feGaussianBlur";

    printValues(os);

    os << "/>\n";
  }
  else
    os << "feGaussianBlur ";
}

void
CSVGFeGaussianBlur::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue(os, "in"          , filterIn_ );
  printNameValue(os, "result"      , filterOut_);

  if      (stdDevX_ && ! stdDevY_)
    printNameValue(os, "stdDeviation", stdDevX_);
  else if (stdDevX_ && stdDevY_)
    os << " stdDeviation=\"" << stdDevX_.value() << " " << stdDevY_.value() << "\"";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeGaussianBlur &filter)
{
  filter.print(os, false);

  return os;
}
