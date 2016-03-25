#include <CSVGFeFlood.h>
#include <CSVG.h>
#include <CRGBName.h>

CSVGFeFlood::
CSVGFeFlood(CSVG &svg) :
 CSVGFilter(svg)
{
}

CSVGFeFlood::
CSVGFeFlood(const CSVGFeFlood &fe) :
 CSVGFilter (fe),
 filter_in_ (fe.filter_in_),
 filter_out_(fe.filter_out_),
 color_     (fe.color_)
{
}

CSVGFeFlood *
CSVGFeFlood::
dup() const
{
  return new CSVGFeFlood(*this);
}

bool
CSVGFeFlood::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "flood-color", str))
    color_ = CRGBName::toRGBA(str);
  else if (svg_.realOption(opt_name, opt_value, "flood-opacity", &real)) {
    CRGBA c = color_.getValue(CRGBA(0,0,0,0));

    c.setAlpha(real);

    color_ = c;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeFlood::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_.getValue("SourceGraphic"));

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_.getValue("SourceGraphic"), dst_image);
}

CImagePtr
CSVGFeFlood::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  dst_image->setRGBAData(color_.getValue(CRGBA(0,0,0,0)));

  return dst_image;
}

void
CSVGFeFlood::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feFlood";

    printNameValue(os, "id", id_);

    printNameValue(os, "in"    , filter_in_ );
    printNameValue(os, "result", filter_out_);

    if (color_.isValid())
      os << " flood-color=\"" << color_.getValue().getRGB().stringEncode() << "\"";

    os << "/>" << std::endl;
  }
  else
    os << "feFlood ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeFlood &fe)
{
  fe.print(os, false);

  return os;
}
