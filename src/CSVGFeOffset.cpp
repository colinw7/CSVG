#include <CSVGFeOffset.h>
#include <CSVG.h>

CSVGFeOffset::
CSVGFeOffset(CSVG &svg) :
 CSVGFilter(svg)
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

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
  CImagePtr src_image = svg_.getBufferImage(filter_in_.getValue("SourceGraphic"));

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_.getValue("SourceGraphic"), dst_image);
}

CImagePtr
CSVGFeOffset::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  dst_image->setRGBAData(CRGBA(1,1,1,1));

  int dx, dy;

  svg_.lengthToPixel(dx_.getValue(0), dy_.getValue(0), &dx, &dy);

  dst_image->subCopyFrom(src_image, 0, 0, -1, -1, dx, dy);

  return dst_image;
}

void
CSVGFeOffset::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feOffset";

    if (filter_in_.isValid())
      os << " in=\"" << filter_in_.getValue() << "\"";

    if (filter_out_.isValid())
      os << " result=\"" << filter_out_.getValue() << "\"";

    if (dx_.isValid())
      os << " dx=\"" << dx_.getValue() << "\"";

    if (dy_.isValid())
      os << " dy=\"" << dy_.getValue() << "\"";

    os << "/>" << std::endl;
  }
  else
    os << "feOffset ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeOffset &filter)
{
  filter.print(os, false);

  return os;
}
