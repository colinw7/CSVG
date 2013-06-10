#include "CSVGI.h"

CSVGFeOffset::
CSVGFeOffset(CSVG &svg) :
 CSVGFilter (svg),
 filter_in_ ("SourceGraphic"),
 filter_out_("SourceGraphic"),
 dx_        (0),
 dy_        (0)
{
}

CSVGFeOffset::
CSVGFeOffset(const CSVGFeOffset &offset) :
 CSVGFilter (offset),
 filter_in_ (offset.filter_in_),
 filter_out_(offset.filter_out_),
 dx_        (offset.dx_),
 dy_        (offset.dy_)
{
}

CSVGFeOffset *
CSVGFeOffset::
dup() const
{
  return new CSVGFeOffset(*this);
}

bool
CSVGFeOffset::
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else if (svg_.realOption  (opt_name, opt_value, "dx", &real))
    dx_ = real;
  else if (svg_.realOption  (opt_name, opt_value, "dy", &real))
    dy_ = real;
  else
    return CSVGFilter::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeOffset::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_);

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_, dst_image);
}

CImagePtr
CSVGFeOffset::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  dst_image->setRGBAData(CRGBA(1,1,1,1));

  int dx, dy;

  svg_.lengthToPixel(dx_, dy_, &dx, &dy);

  dst_image->subCopyFrom(src_image, 0, 0, -1, -1, dx, dy);

  return dst_image;
}

void
CSVGFeOffset::
print(ostream &os) const
{
  os << "feOffset ";
}

ostream &
operator<<(ostream &os, const CSVGFeOffset &filter)
{
  filter.print(os);

  return os;
}
