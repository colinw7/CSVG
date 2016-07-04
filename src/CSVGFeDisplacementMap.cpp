#include <CSVGFeDisplacementMap.h>
#include <CSVGBuffer.h>
#include <CSVGFilter.h>
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

std::string
CSVGFeDisplacementMap::
getFilterIn1() const
{
  return calcFilterIn(filterIn1_);
}

std::string
CSVGFeDisplacementMap::
getFilterIn2() const
{
  return calcFilterIn(filterIn2_);
}

std::string
CSVGFeDisplacementMap::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
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
drawElement()
{
  CSVGBuffer *inBuffer1 = svg_.getBuffer(getFilterIn1());
  CSVGBuffer *inBuffer2 = svg_.getBuffer(getFilterIn2());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing1 = inBuffer1->isDrawing();
  bool inDrawing2 = inBuffer2->isDrawing();

  if (inDrawing1) inBuffer1->stopDraw();
  if (inDrawing2) inBuffer2->stopDraw();

  //---

  // get filtered object coords
  CBBox2D inBBox1, inBBox2;

  getBufferSubRegion(inBuffer1, inBBox1);
  getBufferSubRegion(inBuffer2, inBBox2);

  CBBox2D inBBox = inBBox1 + inBBox2;

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer1 = svg_.getBuffer(objectBufferName + "_in1");
    CSVGBuffer *buffer2 = svg_.getBuffer(objectBufferName + "_in2");

    buffer1->setImageBuffer(inBuffer1);
    buffer1->setBBox       (inBBox1);

    buffer2->setImageBuffer(inBuffer2);
    buffer2->setBBox       (inBBox2);
  }

  //---

  CSVGBuffer::displacementMapBuffers(inBuffer1, inBuffer2, inBBox,
                                     getXChannel(), getYChannel(), getScale(), outBuffer);

  outBuffer->setBBox(inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  if (inDrawing1) inBuffer1->startDraw();
  if (inDrawing2) inBuffer2->startDraw();

  return true;
}

void
CSVGFeDisplacementMap::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feDisplacementMap";

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "feDisplacementMap ";
}

void
CSVGFeDisplacementMap::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue(os, "in"              , filterIn1_);
  printNameValue(os, "in2"             , filterIn2_);
  printNameValue(os, "scale"           , scale_);
  printNameValue(os, "xChannelSelector", xChannelSelector_);
  printNameValue(os, "yChannelSelector", yChannelSelector_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeDisplacementMap &filter)
{
  filter.print(os, false);

  return os;
}
