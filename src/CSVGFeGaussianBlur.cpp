#include "CSVGI.h"

CSVGFeGaussianBlur::
CSVGFeGaussianBlur(CSVG &svg) :
 CSVGFilter (svg),
 filter_in_ (),
 filter_out_(),
 std_dev_   ()
{
  std_dev_    = 0;
  filter_in_  = "SourceGraphic";
  filter_out_ = "SourceGraphic";
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
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;

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
  CImagePtr src_image = svg_.getBufferImage(filter_in_);

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_, dst_image);
}

CImagePtr
CSVGFeGaussianBlur::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  src_image->gaussianBlur(dst_image, std_dev_, std_dev_);

  return dst_image;
}

void
CSVGFeGaussianBlur::
print(ostream &os) const
{
  os << "feGaussianBlur ";
}

ostream &
operator<<(ostream &os, const CSVGFeGaussianBlur &filter)
{
  filter.print(os);

  return os;
}
