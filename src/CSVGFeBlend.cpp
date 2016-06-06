#include <CSVGFeBlend.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeBlend::
CSVGFeBlend(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeBlend::
CSVGFeBlend(const CSVGFeBlend &fe) :
 CSVGFilterBase(fe),
 mode_     (fe.mode_),
 filterIn1_(fe.filterIn1_),
 filterIn2_(fe.filterIn2_),
 filterOut_(fe.filterOut_)
{
}

CSVGFeBlend *
CSVGFeBlend::
dup() const
{
  return new CSVGFeBlend(*this);
}

bool
CSVGFeBlend::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn1_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "in2", str))
    filterIn2_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "mode", str)) {
    if      (str == "normal"  ) mode_ = CSVGBlendMode::NORMAL;
    else if (str == "multiply") mode_ = CSVGBlendMode::MULTIPLY;
    else if (str == "screen"  ) mode_ = CSVGBlendMode::SCREEN;
    else if (str == "darken"  ) mode_ = CSVGBlendMode::DARKEN;
    else if (str == "lighten" ) mode_ = CSVGBlendMode::LIGHTEN;
  }
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeBlend::
draw()
{
  CSVGBuffer *inBuffer1 = svg_.getBuffer(getFilterIn1());
  CSVGBuffer *inBuffer2 = svg_.getBuffer(getFilterIn2());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer1 = svg_.getBuffer(objectBufferName + "_in1");
    CSVGBuffer *buffer2 = svg_.getBuffer(objectBufferName + "_in2");

    buffer1->setImageBuffer(inBuffer1);
    buffer2->setImageBuffer(inBuffer2);
  }

  filterImage(inBuffer1, inBuffer2, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
  }

  return true;
}

void
CSVGFeBlend::
filterImage(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, CSVGBuffer *outBuffer)
{
  CSVGBuffer::blendBuffers(inBuffer1, inBuffer2, getMode(), outBuffer);
}

void
CSVGFeBlend::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feBlend";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    if (mode_.isValid()) {
      CSVGBlendMode mode = getMode();

      if      (mode == CSVGBlendMode::NORMAL  ) os << " mode=\"normal\"";
      else if (mode == CSVGBlendMode::MULTIPLY) os << " mode=\"multiply\"";
      else if (mode == CSVGBlendMode::SCREEN  ) os << " mode=\"screen\"";
      else if (mode == CSVGBlendMode::DARKEN  ) os << " mode=\"darken\"";
      else if (mode == CSVGBlendMode::LIGHTEN ) os << " mode=\"lighten\"";
    }

    printNameValue(os, "in"    , filterIn1_);
    printNameValue(os, "in2"   , filterIn2_);
    printNameValue(os, "result", filterOut_);

    os << "/>" << std::endl;
  }
  else
    os << "feBlend ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeBlend &fe)
{
  fe.print(os, false);

  return os;
}
