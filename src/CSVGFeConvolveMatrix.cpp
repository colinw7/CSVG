#include <CSVGFeConvolveMatrix.h>
#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeConvolveMatrix::
CSVGFeConvolveMatrix(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeConvolveMatrix::
CSVGFeConvolveMatrix(const CSVGFeConvolveMatrix &fe) :
 CSVGFilterBase(fe)
{
}

CSVGFeConvolveMatrix *
CSVGFeConvolveMatrix::
dup() const
{
  return new CSVGFeConvolveMatrix(*this);
}

std::string
CSVGFeConvolveMatrix::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

bool
CSVGFeConvolveMatrix::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  double      r;
  std::string str;
  Reals       reals;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.realListOption(opt_name, opt_value, "order", reals))
    order_ = reals;
  else if (svg_.realListOption(opt_name, opt_value, "kernelMatrix", reals))
    kernelMatrix_ = reals;
  else if (svg_.realOption(opt_name, opt_value, "divisor", &r))
    divisor_ = r;
  else if (svg_.realOption(opt_name, opt_value, "bias", &r))
    bias_ = r;
  else if (svg_.realOption(opt_name, opt_value, "targetX", &r))
    targetX_ = r;
  else if (svg_.realOption(opt_name, opt_value, "targetY", &r))
    targetY_ = r;
  else if (svg_.stringOption(opt_name, opt_value, "edgeMode", str))
    edgeMode_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "kernelUnitLength", str))
    kernelUnitLength_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "preserveAlpha", str))
    preserveAlpha_ = str;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeConvolveMatrix::
drawElement()
{
  auto *inBuffer = svg_.getBuffer(getFilterIn());

  bool inDrawing = inBuffer->isDrawing();

  if (inDrawing) inBuffer->stopDraw();

  //---

  // get filtered object coords
  CBBox2D bbox;

  getBufferSubRegion(inBuffer, bbox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer = svg_.getBuffer(objectBufferName + "_in1");

    buffer->setImageBuffer(inBuffer);
    buffer->setBBox       (bbox);
  }

  //---

  CSVGConvolveData convolveData;

  if (order_) {
    if      (order_.value().size() > 1) {
      convolveData.xorder = order_.value()[0];
      convolveData.yorder = order_.value()[1];
    }
    else if (order_.value().size() > 0) {
      convolveData.xorder = order_.value()[0];
      convolveData.yorder = convolveData.xorder;
    }
  }

  convolveData.kernelMatrix     = getKernelMatrix();
  convolveData.divisor          = getDivisor();
  convolveData.bias             = getBias();
  convolveData.targetX          = getTargetX();
  convolveData.targetY          = getTargetY();
  convolveData.edgeMode         = getEdgeMode();
  convolveData.kernelUnitLength = getKernelUnitLength();
  convolveData.preserveAlpha    = (getPreserveAlpha() == "true");

  CSVGBuffer::convolveMatrixBuffers(inBuffer, bbox, convolveData);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(inBuffer);
    buffer->setBBox       (bbox);
  }

  //---

  if (inDrawing) inBuffer->startDraw();

  return true;
}

void
CSVGFeConvolveMatrix::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feConvolveMatrix";

    printValues(os);

    os << "/>\n";
  }
  else
    os << "feConvolveMatrix ";
}

void
CSVGFeConvolveMatrix::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue (os, "in"              , filterIn_);
  printNameValues(os, "order"           , order_);
  printNameValues(os, "kernelMatrix"    , kernelMatrix_);
  printNameValue (os, "divisor"         , divisor_);
  printNameValue (os, "bias"            , bias_);
  printNameValue (os, "targetX"         , targetX_);
  printNameValue (os, "targetY"         , targetY_);
  printNameValue (os, "edgeMode"        , edgeMode_);
  printNameValue (os, "kernelUnitLength", kernelUnitLength_);
  printNameValue (os, "preserveAlpha"   , preserveAlpha_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeConvolveMatrix &filter)
{
  filter.print(os, false);

  return os;
}
