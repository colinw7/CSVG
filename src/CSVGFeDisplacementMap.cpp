#include <CSVGFeDisplacementMap.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeDisplacementMap::
CSVGFeDisplacementMap(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeDisplacementMap::
CSVGFeDisplacementMap(const CSVGFeDisplacementMap &fe) :
 CSVGFilterBase(fe)
{
}

CSVGFeDisplacementMap *
CSVGFeDisplacementMap::
dup() const
{
  return new CSVGFeDisplacementMap(*this);
}

bool
CSVGFeDisplacementMap::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      r;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn1_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "in2", str))
    filterIn2_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.realOption(opt_name, opt_value, "scale", &r))
    scale_ = r;
  else if (svg_.stringOption(opt_name, opt_value, "xChannelSelector", str))
    xChannelSelector_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "yChannelSelector", str))
    yChannelSelector_ = str;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeDisplacementMap::
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
CSVGFeDisplacementMap::
filterImage(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, CSVGBuffer *outBuffer)
{
  CSVGBuffer::displacementMapBuffers(inBuffer1, inBuffer2, getXChannel(), getYChannel(),
                                     getScale(), outBuffer);
}

void
CSVGFeDisplacementMap::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feDisplacementMap";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "in"              , filterIn1_);
    printNameValue(os, "in2"             , filterIn2_);
    printNameValue(os, "scale"           , scale_);
    printNameValue(os, "xChannelSelector", xChannelSelector_);
    printNameValue(os, "yChannelSelector", yChannelSelector_);

    os << "/>" << std::endl;
  }
  else
    os << "feDisplacementMap ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeDisplacementMap &filter)
{
  filter.print(os, false);

  return os;
}
