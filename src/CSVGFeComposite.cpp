#include <CSVGFeComposite.h>
#include <CSVGBuffer.h>
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

void
CSVGFeComposite::
draw()
{
  CSVGBuffer *inBuffer1 = svg_.getBuffer(getFilterIn1());
  CSVGBuffer *inBuffer2 = svg_.getBuffer(getFilterIn2());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer1 = svg_.getBuffer(objectBufferName + "_in1");

    buffer1->setImage(inBuffer1->getImage());

    CSVGBuffer *buffer2 = svg_.getBuffer(objectBufferName + "_in2");

    buffer2->setImage(inBuffer2->getImage());
  }

  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  filterImage(inBuffer1, inBuffer2, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
  }
}

void
CSVGFeComposite::
filterImage(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, CSVGBuffer *outBuffer)
{
  CImagePtr src_image1 = inBuffer1->getImage();
  CImagePtr src_image2 = inBuffer2->getImage();

  if (! src_image1.isValid() || ! src_image2.isValid())
    return;

  CImagePtr dst_image = src_image1->dup();

  CRGBACombineDef def;

  def.src_mode = CRGBA_COMBINE_ONE;
  def.dst_mode = CRGBA_COMBINE_ONE;
  def.func     = getType();
  def.k1       = getK1();
  def.k2       = getK2();
  def.k3       = getK3();
  def.k4       = getK4();

  dst_image->combine(src_image2, def);

  outBuffer->setImage(dst_image);
}

void
CSVGFeComposite::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feComposite";

    CSVGObject::printValues(os);

    printNameValue(os, "in"    , filterIn1_);
    printNameValue(os, "in2"   , filterIn2_);
    printNameValue(os, "result", filterOut_);

    if (type_.isValid()) {
      os << " operator=\"";

      if      (type_.getValue() == CRGBA_COMBINE_OVER      ) os << "over";
      else if (type_.getValue() == CRGBA_COMBINE_IN        ) os << "in";
      else if (type_.getValue() == CRGBA_COMBINE_OUT       ) os << "out";
      else if (type_.getValue() == CRGBA_COMBINE_ATOP      ) os << "atop";
      else if (type_.getValue() == CRGBA_COMBINE_XOR       ) os << "xor";
      else if (type_.getValue() == CRGBA_COMBINE_ARITHMETIC) os << "arithmetic";

      os << "\"";
    }

    printNameValue(os, "k1", k1_);
    printNameValue(os, "k2", k2_);
    printNameValue(os, "k3", k3_);
    printNameValue(os, "k4", k4_);

    os << "/>" << std::endl;
  }
  else
    os << "feComposite ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeComposite &fe)
{
  fe.print(os, false);

  return os;
}
