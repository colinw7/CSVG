#include <CSVGFeTurbulence.h>
#include <CSVG.h>

CSVGFeTurbulence::
CSVGFeTurbulence(CSVG &svg) :
 CSVGFilter (svg)
{
}

CSVGFeTurbulence::
CSVGFeTurbulence(const CSVGFeTurbulence &turb) :
 CSVGFilter (turb),
 type_      (turb.type_),
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;
  long        integer;

  if      (svg_.stringOption(opt_name, opt_value, "type", str))
    type_ = str;
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
  CImagePtr src_image = svg_.getBufferImage(filter_in_.getValue("SourceGraphic"));
  CImagePtr dst_image = svg_.getBufferImage(filter_out_.getValue("SourceGraphic"));

  CImagePtr dst_image1 = dst_image;

  if (src_image == dst_image)
    dst_image1 = dst_image->dup();

  dst_image1->turbulence(isFractalNoise(), baseFreq_.getValue(0.1),
                         numOctaves_.getValue(1), seed_.getValue(0));

  if (src_image == dst_image)
    dst_image->copyFrom(dst_image1);
}

CImagePtr
CSVGFeTurbulence::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  dst_image->turbulence(isFractalNoise(), baseFreq_.getValue(0.1),
                        numOctaves_.getValue(1), seed_.getValue(0));

  return dst_image;
}

void
CSVGFeTurbulence::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feTurbulence";

    CSVGObject::printValues(os);

    printNameValue(os, "type"         , type_);
    printNameValue(os, "baseFrequency", baseFreq_);
    printNameValue(os, "numOctaves"   , numOctaves_);
    printNameValue(os, "seed"         , seed_);
    printNameValue(os, "in"           , filter_in_);
    printNameValue(os, "result"       , filter_out_);

    os << "/>" << std::endl;
  }
  else
    os << "feTurbulence ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeTurbulence &filter)
{
  filter.print(os, false);

  return os;
}
