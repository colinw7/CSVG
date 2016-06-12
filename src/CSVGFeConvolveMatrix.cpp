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
draw()
{
  CSVGBuffer *inBuffer = svg_.getBuffer(getFilterIn());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in1");

    buffer->setImageBuffer(inBuffer);
  }

  // TODO: handle divisor, bias, targetX, targetY, edgeMode, kernelUnitLength, preserveAlpha,

  double xorder = 3;
  double yorder = 3;

  if (order_.isValid()) {
    if      (order_.getValue().size() > 1) {
      xorder = order_.getValue()[0];
      yorder = order_.getValue()[1];
    }
    else if (order_.getValue().size() > 0) {
      xorder = order_.getValue()[0];
      yorder = xorder;
    }
  }

  bool preserveAlpha = (this->preserveAlpha() == "true");

  CSVGBuffer::convolveMatrixBuffers(inBuffer, xorder, yorder, getKernelMatrix(), preserveAlpha);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(inBuffer);
  }

  return true;
}

void
CSVGFeConvolveMatrix::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feConvolveMatrix";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

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

    os << "/>" << std::endl;
  }
  else
    os << "feConvolveMatrix ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeConvolveMatrix &filter)
{
  filter.print(os, false);

  return os;
}
