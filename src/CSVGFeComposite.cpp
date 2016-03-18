#include <CSVGI.h>

CSVGFeComposite::
CSVGFeComposite(CSVG &svg) :
 CSVGFilter (svg),
 type_      (),
 filter_in1_(),
 filter_in2_(),
 filter_out_()
{
  type_       = CRGBA_COMBINE_OVER;
  filter_in1_ = "SourceGraphic";
  filter_in2_ = "SourceGraphic";
  filter_out_ = "SourceGraphic";
}

CSVGFeComposite::
CSVGFeComposite(const CSVGFeComposite &fe) :
 CSVGFilter (fe),
 type_      (fe.type_      ),
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
    ;
  else if (svg_.realOption(opt_name, opt_value, "k2", &real))
    ;
  else if (svg_.realOption(opt_name, opt_value, "k3", &real))
    ;
  else if (svg_.realOption(opt_name, opt_value, "k4", &real))
    ;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeComposite::
draw()
{
  CImagePtr src_image1 = svg_.getBufferImage(filter_in1_);
  CImagePtr src_image2 = svg_.getBufferImage(filter_in2_);

  CImagePtr dst_image = filterImage2(src_image1, src_image2);

  svg_.setBufferImage(filter_out_, dst_image);
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
  def.func     = type_;

  dst_image2->combine(src_image1, def);

  return dst_image2;
}

void
CSVGFeComposite::
print(std::ostream &os) const
{
  os << "feComposite ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeComposite &fe)
{
  fe.print(os);

  return os;
}
