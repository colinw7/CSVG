#include <CSVGI.h>

CSVGFeColorMatrix::
CSVGFeColorMatrix(CSVG &svg) :
 CSVGFilter (svg),
 filter_in_ (),
 filter_out_(),
 type_      (),
 values_    ()
{
  filter_in_  = "SourceGraphic";
  filter_out_ = "SourceGraphic";

  type_ = MATRIX_TYPE;
}

CSVGFeColorMatrix::
CSVGFeColorMatrix(const CSVGFeColorMatrix &blur) :
 CSVGFilter (blur),
 filter_in_ (blur.filter_in_),
 filter_out_(blur.filter_out_),
 type_      (blur.type_),
 values_    (blur.values_)
{
}

CSVGFeColorMatrix *
CSVGFeColorMatrix::
dup() const
{
  return new CSVGFeColorMatrix(*this);
}

bool
CSVGFeColorMatrix::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string         str;
  std::vector<double> reals;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "type", str)) {
    if      (str == "matrix")
      type_ = MATRIX_TYPE;
    else if (str == "saturate")
      type_ = SATURATE_TYPE;
    else if (str == "hueRotate")
      type_ = HUE_ROTATE_TYPE;
    else if (str == "luminanceToAlpha")
      type_ = LUMINANCE_TO_ALPHA_TYPE;
  }
  else if (svg_.realListOption(opt_name, opt_value, "values", reals)) {
    values_ = reals;

    uint num_values = values_.size();

    if (type_ == MATRIX_TYPE && num_values != 20)
      std::cerr << "Invalid matrix values" << std::endl;
    if (type_ == SATURATE_TYPE && num_values != 1)
      std::cerr << "Invalid saturate values" << std::endl;
    if (type_ == HUE_ROTATE_TYPE && num_values != 1)
      std::cerr << "Invalid hueRotate values" << std::endl;
  }
  else
    return CSVGFilter::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeColorMatrix::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_);

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_, dst_image);
}

CImagePtr
CSVGFeColorMatrix::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  if      (type_ == MATRIX_TYPE)
    dst_image->applyColorMatrix(&values_[0]);
  else if (type_ == SATURATE_TYPE)
    dst_image->saturate(values_[0]);
  else if (type_ == HUE_ROTATE_TYPE)
    dst_image->rotateHue(values_[0]);
  else if (type_ == LUMINANCE_TO_ALPHA_TYPE)
    dst_image->luminanceToAlpha();

  return dst_image;
}

void
CSVGFeColorMatrix::
print(std::ostream &os) const
{
  os << "feColorMatrix ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeColorMatrix &filter)
{
  filter.print(os);

  return os;
}
