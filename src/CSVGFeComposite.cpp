#include <CSVGFeComposite.h>
#include <CSVG.h>

CSVGFeComposite::
CSVGFeComposite(CSVG &svg) :
 CSVGFilter(svg)
{
}

CSVGFeComposite::
CSVGFeComposite(const CSVGFeComposite &fe) :
 CSVGFilter (fe),
 type_      (fe.type_),
 filter_in1_(fe.filter_in1_),
 filter_in2_(fe.filter_in2_),
 filter_out_(fe.filter_out_)
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
    filter_in1_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "in2", str))
    filter_in2_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
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
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeComposite::
draw()
{
  CImagePtr src_image1 = svg_.getBufferImage(filter_in1_.getValue("SourceGraphic"));
  CImagePtr src_image2 = svg_.getBufferImage(filter_in2_.getValue("SourceGraphic"));

  CImagePtr dst_image = filterImage2(src_image1, src_image2);

  svg_.setBufferImage(filter_out_.getValue("SourceGraphic"), dst_image);
}

CImagePtr
CSVGFeComposite::
filterImage2(CImagePtr src_image1, CImagePtr src_image2)
{
  if (! src_image1.isValid() || ! src_image2.isValid())
    return CImagePtr();

  CImagePtr dst_image2 = src_image2->dup();

  CRGBACombineDef def;

  def.src_mode = CRGBA_COMBINE_ONE;
  def.dst_mode = CRGBA_COMBINE_ONE;
  def.func     = type_.getValue(CRGBA_COMBINE_OVER);

  dst_image2->combine(src_image1, def);

  return dst_image2;
}

void
CSVGFeComposite::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feComposite";

    CSVGObject::printValues(os);

    printNameValue(os, "in"    , filter_in1_);
    printNameValue(os, "in2"   , filter_in2_);
    printNameValue(os, "result", filter_out_);

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
