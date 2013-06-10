#include "CSVGI.h"

CSVGFeFlood::
CSVGFeFlood(CSVG &svg) :
 CSVGFilter (svg),
 filter_in_ (),
 filter_out_(),
 color_     ()
{
  filter_in_  = "SourceGraphic";
  filter_out_ = "SourceGraphic";
  color_      = CRGBA(0,0,0,0);
}

CSVGFeFlood::
CSVGFeFlood(const CSVGFeFlood &fe) :
 CSVGFilter (fe),
 filter_in_ (fe.filter_in_ ),
 filter_out_(fe.filter_out_),
 color_     (fe.color_     )
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
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "flood-color", str)) {
    color_ = CRGBName::toRGBA(str);
  }
  else if (svg_.realOption(opt_name, opt_value, "flood-opacity", &real))
    color_.setAlpha(real);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeFlood::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_);

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_, dst_image);
}

CImagePtr
CSVGFeFlood::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  dst_image->setRGBAData(color_);

  return dst_image;
}

void
CSVGFeFlood::
print(ostream &os) const
{
  os << "feFlood ";
}

ostream &
operator<<(ostream &os, const CSVGFeFlood &fe)
{
  fe.print(os);

  return os;
}
