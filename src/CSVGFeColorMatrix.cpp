#include <CSVGFeColorMatrix.h>
#include <CSVGBuffer.h>
#include <CSVGFilter.h>
#include <CSVG.h>

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

std::string
CSVGFeColorMatrix::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeColorMatrix::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
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

  CSVGBuffer::colorMatrixBuffers(inBuffer, inBBox, getType(), getValues(), outBuffer);

  outBuffer->setBBox(inBBox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (inBBox);
  }

  //---

  if (inDrawing) inBuffer->startDraw();

  return true;
}

void
CSVGFeColorMatrix::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feColorMatrix";

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "feColorMatrix ";
}

void
CSVGFeColorMatrix::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

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
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeColorMatrix &filter)
{
  filter.print(os, false);

  return os;
}
