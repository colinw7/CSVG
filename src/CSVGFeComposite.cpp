#include <CSVGFeComposite.h>
#include <CSVGBuffer.h>
#include <CSVGFilter.h>
#include <CSVG.h>

CSVGFeComposite::
CSVGFeComposite(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeComposite::
CSVGFeComposite(const CSVGFeComposite &fe) :
 CSVGFilterBase (fe),
 type_     (fe.type_),
 filterIn1_(fe.filterIn1_),
 filterIn2_(fe.filterIn2_),
 filterOut_(fe.filterOut_)
{
}

CSVGFeComposite *
CSVGFeComposite::
dup() const
{
  return new CSVGFeComposite(*this);
}

std::string
CSVGFeComposite::
getFilterIn1() const
{
  return calcFilterIn(filterIn1_);
}

std::string
CSVGFeComposite::
getFilterIn2() const
{
  return calcFilterIn(filterIn2_);
}

std::string
CSVGFeComposite::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
}

bool
CSVGFeComposite::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn1_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "in2", str))
    filterIn2_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "operator", str)) {
    if      (str == "over"      ) type_ = CRGBA_COMBINE_OVER;
    else if (str == "in"        ) type_ = CRGBA_COMBINE_IN;
    else if (str == "out"       ) type_ = CRGBA_COMBINE_OUT;
    else if (str == "atop"      ) type_ = CRGBA_COMBINE_ATOP;
    else if (str == "xor"       ) type_ = CRGBA_COMBINE_XOR;
    else if (str == "arithmetic") type_ = CRGBA_COMBINE_ARITHMETIC;
  }
  else if (svg_.realOption(opt_name, opt_value, "k1", &real))
    k1_ = real;
  else if (svg_.realOption(opt_name, opt_value, "k2", &real))
    k2_ = real;
  else if (svg_.realOption(opt_name, opt_value, "k3", &real))
    k3_ = real;
  else if (svg_.realOption(opt_name, opt_value, "k4", &real))
    k4_ = real;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeComposite::
drawElement()
{
  auto *inBuffer1 = svg_.getBuffer(getFilterIn1());
  auto *inBuffer2 = svg_.getBuffer(getFilterIn2());
  auto *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing1 = inBuffer1->isDrawing();
  bool inDrawing2 = inBuffer2->isDrawing();

  if (inDrawing1) inBuffer1->stopDraw();
  if (inDrawing2) inBuffer2->stopDraw();

  //---

  // get filtered object coords
  CBBox2D inBBox1, inBBox2;

  getBufferSubRegion(inBuffer1, inBBox1);
  getBufferSubRegion(inBuffer2, inBBox2);

  auto inBBox = inBBox1 + inBBox2;

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer1 = svg_.getBuffer(objectBufferName + "_in1");
    auto *buffer2 = svg_.getBuffer(objectBufferName + "_in2");

    buffer1->setImageBuffer(inBuffer1);
    buffer1->setBBox       (inBBox1);

    buffer2->setImageBuffer(inBuffer2);
    buffer2->setBBox       (inBBox2);
  }

  //---

  CSVGBuffer::compositeBuffers(inBuffer1, inBuffer2, inBBox, getType(),
                               getK1(), getK2(), getK3(), getK4(), outBuffer);

  outBuffer->setBBox(inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  if (inDrawing1) inBuffer1->startDraw();
  if (inDrawing2) inBuffer2->startDraw();

  return true;
}

void
CSVGFeComposite::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feComposite";

    printValues(os);

    os << "/>\n";
  }
  else
    os << "feComposite ";
}

void
CSVGFeComposite::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue(os, "in"    , filterIn1_);
  printNameValue(os, "in2"   , filterIn2_);
  printNameValue(os, "result", filterOut_);

  if (type_) {
    os << " operator=\"";

    if      (type_.value() == CRGBA_COMBINE_OVER      ) os << "over";
    else if (type_.value() == CRGBA_COMBINE_IN        ) os << "in";
    else if (type_.value() == CRGBA_COMBINE_OUT       ) os << "out";
    else if (type_.value() == CRGBA_COMBINE_ATOP      ) os << "atop";
    else if (type_.value() == CRGBA_COMBINE_XOR       ) os << "xor";
    else if (type_.value() == CRGBA_COMBINE_ARITHMETIC) os << "arithmetic";

    os << "\"";
  }

  printNameValue(os, "k1", k1_);
  printNameValue(os, "k2", k2_);
  printNameValue(os, "k3", k3_);
  printNameValue(os, "k4", k4_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeComposite &fe)
{
  fe.print(os, false);

  return os;
}
