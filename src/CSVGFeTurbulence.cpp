#include <CSVGFeTurbulence.h>
#include <CSVGFilter.h>
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
 baseFreqX_ (turb.baseFreqX_),
 baseFreqY_ (turb.baseFreqY_),
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

std::string
CSVGFeTurbulence::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeTurbulence::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
}

bool
CSVGFeTurbulence::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string         str;
  double              real;
  long                integer;
  std::vector<double> reals;

  if      (svg_.stringOption(opt_name, opt_value, "type", str))
    type_ = str;
  else if (svg_.realListOption(opt_name, opt_value, "baseFrequency", reals)) {
    if      (reals.size() == 1)
      baseFreqX_ = reals[0];
    else if (reals.size() > 1) {
      baseFreqX_ = reals[0];
      baseFreqY_ = reals[1];
    }
  }
  else if (svg_.integerOption(opt_name, opt_value, "numOctaves", &integer))
    numOctaves_ = integer;
  else if (svg_.realOption(opt_name, opt_value, "seed", &real))
    seed_ = real;
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

bool
CSVGFeTurbulence::
drawElement()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  bool inDrawing = inBuffer->isDrawing();

  if (inDrawing) inBuffer->stopDraw();

  //---

  // get filtered object coords
  CBBox2D inBBox;

  getBufferSubRegion(inBuffer, inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  double baseFreqX = getBaseFreqX();
  double baseFreqY = getBaseFreqY(baseFreqX);

  CSVGBuffer::turbulenceBuffers(inBuffer, inBBox, isFractalNoise(), baseFreqX, baseFreqY,
                                getNumOctaves(), getSeed(), outBuffer);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(inBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  if (inDrawing) inBuffer->startDraw();

  return true;
}

void
CSVGFeTurbulence::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feTurbulence";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "type", type_);

    if      (baseFreqX_.isValid() && ! baseFreqY_.isValid())
      printNameValue(os, "baseFrequency", baseFreqX_);
    else if (baseFreqX_.isValid() && baseFreqY_.isValid())
      os << " baseFrequency=\"" << baseFreqX_.getValue() << " " << baseFreqY_.getValue() << "\"";

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
