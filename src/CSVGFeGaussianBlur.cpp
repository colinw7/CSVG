#include <CSVGFeGaussianBlur.h>
#include <CSVG.h>

CSVGFeGaussianBlur::
CSVGFeGaussianBlur(CSVG &svg) :
 CSVGFilter(svg)
{
}

CSVGFeGaussianBlur::
CSVGFeGaussianBlur(const CSVGFeGaussianBlur &blur) :
 CSVGFilter (blur),
 filter_in_ (blur.filter_in_),
 filter_out_(blur.filter_out_),
 std_dev_   (blur.std_dev_)
{
}

CSVGFeGaussianBlur *
CSVGFeGaussianBlur::
dup() const
{
  return new CSVGFeGaussianBlur(*this);
}

bool
CSVGFeGaussianBlur::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else if (svg_.realOption  (opt_name, opt_value, "stdDeviation", &real))
    std_dev_ = real;
  else
    return CSVGFilter::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeGaussianBlur::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_.getValue("SourceGraphic"));

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_.getValue("SourceGraphic"), dst_image);
}

CImagePtr
CSVGFeGaussianBlur::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  src_image->gaussianBlur(dst_image, std_dev_.getValue(0), std_dev_.getValue(0));

  return dst_image;
}

void
CSVGFeGaussianBlur::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feGaussianBlur";

    printNameValue(os, "in"          , filter_in_ );
    printNameValue(os, "result"      , filter_out_);
    printNameValue(os, "stdDeviation", std_dev_   );

    os << "/>" << std::endl;
  }
  else
    os << "feGaussianBlur ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeGaussianBlur &filter)
{
  filter.print(os, false);

  return os;
}
