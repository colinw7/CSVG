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

void
CSVGFeColorMatrix::
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
CSVGFeColorMatrix::
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
{
  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  if      (getType() == CSVGColorMatrixType::MATRIX) {
    dst_image->applyColorMatrix(values_);
  }
  // For type="saturate", ‘values’ is a single real number value (0 to 1).
  // A saturate operation is equivalent to the following matrix operation:
  // | R' |   |0.213+0.787s 0.715-0.715s 0.072-0.072s 0 0 |   | R |
  // | G' |   |0.213-0.213s 0.715+0.285s 0.072-0.072s 0 0 |   | G |
  // | B' | = |0.213-0.213s 0.715-0.715s 0.072+0.928s 0 0 | * | B |
  // | A' |   |           0            0            0 1 0 |   | A |
  // | 1  |   |           0            0            0 0 1 |   | 1 |
  else if (getType() == CSVGColorMatrixType::SATURATE) {
    dst_image->saturate(values_[0]);
  }
  // For type="hueRotate", ‘values’ is a single one real number value (degrees).
  // A hueRotate operation is equivalent to the following matrix operation:
  // | R' |   |a00 a01 a02 0 0 |   | R |
  // | G' |   |a10 a11 a12 0 0 |   | G |
  // | B' | = |a20 a21 a22 0 0 | * | B |
  // | A' |   |  0   0   0 1 0 |   | A |
  // | 1  |   |  0   0   0 0 1 |   | 1 |
  //
  // where the terms a00, a01, etc. are calculated as follows:
  //  | a00 a01 a02 |   [+0.213 +0.715 +0.072]
  //  | a10 a11 a12 | = [+0.213 +0.715 +0.072] +
  //  | a20 a21 a22 |   [+0.213 +0.715 +0.072]
  //
  //                        [+0.787 -0.715 -0.072]
  // cos(hueRotate value) * [-0.213 +0.285 -0.072] +
  //                        [-0.213 -0.715 +0.928]
  //                        [-0.213 -0.715 +0.928]
  // sin(hueRotate value) * [+0.143 +0.140 -0.283]
  //                        [-0.787 +0.715 +0.072]
  // Thus, the upper left term of the hue matrix turns out to be:
  //   .213 + cos(hueRotate value)*.787 - sin(hueRotate value)*.213
  else if (getType() == CSVGColorMatrixType::HUE_ROTATE) {
    dst_image->rotateHue(values_[0]);
  }
  // For type="luminanceToAlpha", ‘values’ is not applicable.
  // A luminanceToAlpha operation is equivalent to the following matrix operation:
  // | R' |   |      0      0      0 0 0 |   | R |
  // | G' |   |      0      0      0 0 0 |   | G |
  // | B' | = |      0      0      0 0 0 | * | B |
  // | A' |   | 0.2125 0.7154 0.0721 0 0 |   | A |
  // | 1  |   |      0      0      0 0 1 |   | 1 |
  else if (getType() == CSVGColorMatrixType::LUMINANCE_TO_ALPHA) {
    dst_image->luminanceToAlpha();
  }

  outBuffer->setImage(dst_image);
}

void
CSVGFeColorMatrix::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feColorMatrix";

    CSVGObject::printValues(os);

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
