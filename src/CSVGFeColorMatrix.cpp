#include <CSVGFeColorMatrix.h>
#include <CSVG.h>
#include <CSVGBuffer.h>

CSVGFeColorMatrix::
CSVGFeColorMatrix(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeColorMatrix::
CSVGFeColorMatrix(const CSVGFeColorMatrix &blur) :
 CSVGFilterBase(blur),
 filterIn_ (blur.filterIn_),
 filterOut_(blur.filterOut_),
 type_     (blur.type_),
 values_   (blur.values_)
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
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "type", str)) {
    if      (str == "matrix")
      type_ = CSVGColorMatrixType::MATRIX;
    else if (str == "saturate")
      type_ = CSVGColorMatrixType::SATURATE;
    else if (str == "hueRotate")
      type_ = CSVGColorMatrixType::HUE_ROTATE;
    else if (str == "luminanceToAlpha")
      type_ = CSVGColorMatrixType::LUMINANCE_TO_ALPHA;
  }
  else if (svg_.realListOption(opt_name, opt_value, "values", reals)) {
    values_ = reals;

    uint num_values = values_.size();

    if (getType() == CSVGColorMatrixType::MATRIX && num_values != 20)
      std::cerr << "Invalid matrix values" << std::endl;
    if (getType() == CSVGColorMatrixType::SATURATE && num_values != 1)
      std::cerr << "Invalid saturate values" << std::endl;
    if (getType() == CSVGColorMatrixType::HUE_ROTATE && num_values != 1)
      std::cerr << "Invalid hueRotate values" << std::endl;
  }
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeColorMatrix::
draw()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImageBuffer(inBuffer);
  }

  filterImage(inBuffer, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
  }

  return true;
}

void
CSVGFeColorMatrix::
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
{
  CSVGBuffer::colorMatrixBuffers(inBuffer, getType(), getValues(), outBuffer);
}

void
CSVGFeColorMatrix::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feColorMatrix";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "in"    , filterIn_ );
    printNameValue(os, "result", filterOut_);

    if (type_.isValid()) {
      CSVGColorMatrixType type = getType();

      os << " type=\"";

      if      (type == CSVGColorMatrixType::MATRIX            ) os << "matrix";
      else if (type == CSVGColorMatrixType::SATURATE          ) os << "saturate";
      else if (type == CSVGColorMatrixType::HUE_ROTATE        ) os << "hueRotate";
      else if (type == CSVGColorMatrixType::LUMINANCE_TO_ALPHA) os << "luminanceToAlpha";

      os << "\"";
    }

    if (! values_.empty()) {
      os << " values=\"";

      int i = 0;

      for (const auto &r : getValues()) {
        if (i > 0) os << " ";

        os << r;

        ++i;
      }

      os << "\"";
    }

    os << "/>" << std::endl;
  }
  else
    os << "feColorMatrix ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeColorMatrix &filter)
{
  filter.print(os, false);

  return os;
}
