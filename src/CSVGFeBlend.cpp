#include <CSVGI.h>

CSVGFeBlend::
CSVGFeBlend(CSVG &svg) :
 CSVGFilter (svg),
 type_      (),
 filter_in1_(),
 filter_in2_(),
 filter_out_()
{
  type_       = NORMAL;
  filter_in1_ = "SourceGraphic";
  filter_in2_ = "SourceGraphic";
  filter_out_ = "SourceGraphic";
}

CSVGFeBlend::
CSVGFeBlend(const CSVGFeBlend &fe) :
 CSVGFilter (fe),
 type_      (fe.type_      ),
 filter_in1_(fe.filter_in1_),
 filter_in2_(fe.filter_in2_),
 filter_out_(fe.filter_out_)
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
    filter_in1_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "in2", str))
    filter_in2_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "mode", str)) {
    if      (str == "normal"  ) type_ = NORMAL;
    else if (str == "multiply") type_ = MULTIPLY;
    else if (str == "screen"  ) type_ = SCREEN;
    else if (str == "darken"  ) type_ = DARKEN;
    else if (str == "lighten" ) type_ = LIGHTEN;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeBlend::
draw()
{
  CImagePtr src_image1 = svg_.getBufferImage(filter_in1_);
  CImagePtr src_image2 = svg_.getBufferImage(filter_in2_);

  CImagePtr dst_image = filterImage2(src_image1, src_image2);

  svg_.setBufferImage(filter_out_, dst_image);
}

CImagePtr
CSVGFeBlend::
filterImage2(CImagePtr src_image1, CImagePtr src_image2)
{
  CImagePtr dst_image = src_image1->dup();

  dst_image->combine(src_image2);

  return dst_image;
}

void
CSVGFeBlend::
print(std::ostream &os) const
{
  os << "feBlend ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeBlend &fe)
{
  fe.print(os);

  return os;
}
