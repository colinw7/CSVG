#include <CSVGFeBlend.h>
#include <CSVG.h>

CSVGFeBlend::
CSVGFeBlend(CSVG &svg) :
 CSVGFilter(svg)
{
}

CSVGFeBlend::
CSVGFeBlend(const CSVGFeBlend &fe) :
 CSVGFilter (fe),
 type_      (fe.type_),
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
    if      (str == "normal"  ) type_ = Type::NORMAL;
    else if (str == "multiply") type_ = Type::MULTIPLY;
    else if (str == "screen"  ) type_ = Type::SCREEN;
    else if (str == "darken"  ) type_ = Type::DARKEN;
    else if (str == "lighten" ) type_ = Type::LIGHTEN;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeBlend::
draw()
{
  CImagePtr src_image1 = svg_.getBufferImage(filter_in1_.getValue("SourceGraphic"));
  CImagePtr src_image2 = svg_.getBufferImage(filter_in2_.getValue("SourceGraphic"));

  CImagePtr dst_image = filterImage2(src_image1, src_image2);

  svg_.setBufferImage(filter_out_.getValue("SourceGraphic"), dst_image);
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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feBlend";

    CSVGObject::printValues(os);

    if (type_.isValid()) {
      Type type = type_.getValue();

      if      (type == Type::NORMAL  ) os << " mode=\"normal\"";
      else if (type == Type::MULTIPLY) os << " mode=\"multiply\"";
      else if (type == Type::SCREEN  ) os << " mode=\"screen\"";
      else if (type == Type::DARKEN  ) os << " mode=\"darken\"";
      else if (type == Type::LIGHTEN ) os << " mode=\"lighten\"";
    }

    printNameValue(os, "in"    , filter_in1_);
    printNameValue(os, "in2"   , filter_in2_);
    printNameValue(os, "result", filter_out_);

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
