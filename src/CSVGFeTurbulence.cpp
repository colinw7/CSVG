#include <CSVGI.h>

CSVGFeTurbulence::
CSVGFeTurbulence(CSVG &svg) :
 CSVGFilter (svg),
 fractal_   (false),
 baseFreq_  (0.1),
 numOctaves_(1),
 seed_      (0),
 filter_in_ ("SourceGraphic"),
 filter_out_("SourceGraphic")
{
}

CSVGFeTurbulence::
CSVGFeTurbulence(const CSVGFeTurbulence &turb) :
 CSVGFilter (turb),
 fractal_   (turb.fractal_),
 baseFreq_  (turb.baseFreq_),
 numOctaves_(turb.numOctaves_),
 seed_      (turb.seed_),
 filter_in_ (turb.filter_in_),
 filter_out_(turb.filter_out_)
{
}

CSVGFeTurbulence *
CSVGFeTurbulence::
dup() const
{
  return new CSVGFeTurbulence(*this);
}

bool
CSVGFeTurbulence::
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;
  long   integer;

  if      (svg_.stringOption(opt_name, opt_value, "type", str)) {
    fractal_ = (str == "fractalNoise");
  }
  else if (svg_.realOption   (opt_name, opt_value, "baseFrequency", &real))
    baseFreq_ = real;
  else if (svg_.integerOption(opt_name, opt_value, "numOctaves", &integer))
    numOctaves_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "seed", &integer))
    seed_ = integer;
  else if (svg_.stringOption (opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "result", str))
    filter_out_ = str;
  else
    return CSVGFilter::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeTurbulence::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_);
  CImagePtr dst_image = svg_.getBufferImage(filter_out_);

  CImagePtr dst_image1 = dst_image;

  if (src_image == dst_image)
    dst_image1 = dst_image->dup();

  dst_image1->turbulence(fractal_, baseFreq_, numOctaves_, seed_);

  if (src_image == dst_image)
    dst_image->copyFrom(dst_image1);
}

CImagePtr
CSVGFeTurbulence::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  dst_image->turbulence(fractal_, baseFreq_, numOctaves_, seed_);

  return dst_image;
}

void
CSVGFeTurbulence::
print(ostream &os) const
{
  os << "feTurbulence ";
}

ostream &
operator<<(ostream &os, const CSVGFeTurbulence &filter)
{
  filter.print(os);

  return os;
}
