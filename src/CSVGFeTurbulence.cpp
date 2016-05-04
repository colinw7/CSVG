#include <CSVGFeTurbulence.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeTurbulence::
CSVGFeTurbulence(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeTurbulence::
CSVGFeTurbulence(const CSVGFeTurbulence &turb) :
 CSVGFilterBase(turb),
 type_      (turb.type_),
 baseFreq_  (turb.baseFreq_),
 numOctaves_(turb.numOctaves_),
 seed_      (turb.seed_),
 filterIn_  (turb.filterIn_),
 filterOut_ (turb.filterOut_)
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

  if      (svg_.stringOption (opt_name, opt_value, "type", str))
    type_ = str;
  else if (svg_.realOption   (opt_name, opt_value, "baseFrequency", &real))
    baseFreq_ = real;
  else if (svg_.integerOption(opt_name, opt_value, "numOctaves", &integer))
    numOctaves_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "seed", &integer))
    seed_ = integer;
  else if (svg_.stringOption (opt_name, opt_value, "stitchTiles", str))
    stitchTiles_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "result", str))
    filterOut_ = str;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeTurbulence::
draw()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImage(inBuffer->getImage());
  }

  filterImage(inBuffer, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
  }
}

void
CSVGFeTurbulence::
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
{
  CSVGBuffer::turbulenceBuffers(inBuffer, isFractalNoise(), getBaseFreq(),
                                getNumOctaves(), getSeed(), outBuffer);
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
    printNameValue(os, "stitchTiles"  , stitchTiles_);
    printNameValue(os, "in"           , filterIn_);
    printNameValue(os, "result"       , filterOut_);

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
