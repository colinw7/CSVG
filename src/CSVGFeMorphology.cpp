#include <CSVGFeMorphology.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFeMorphology::
CSVGFeMorphology(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeMorphology::
CSVGFeMorphology(const CSVGFeMorphology &fe) :
 CSVGFilterBase(fe)
{
}

CSVGFeMorphology *
CSVGFeMorphology::
dup() const
{
  return new CSVGFeMorphology(*this);
}

bool
CSVGFeMorphology::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "class", str))
    class_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "style", str))
    style_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "operator", str)) {
    if      (str == "erode" ) operator_ = CSVGMorphologyOperator::ERODE;
    else if (str == "dilate") operator_ = CSVGMorphologyOperator::DILATE;
  }
  else if (svg_.stringOption(opt_name, opt_value, "radius", str))
    radius_ = str;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeMorphology::
draw()
{
  CSVGBuffer *inBuffer = svg_.getBuffer(getFilterIn());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImage(inBuffer->getImage());
  }

  filterImage(inBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(inBuffer->getImage());
  }
}

void
CSVGFeMorphology::
filterImage(CSVGBuffer *inBuffer)
{
  CImagePtr src_image = inBuffer->getImage();

  CImagePtr dst_image;

  CSVGMorphologyOperator op = getOperator();

  if      (op == CSVGMorphologyOperator::ERODE)
    dst_image = src_image->erode(/*isAlpha*/true);
  else if (op == CSVGMorphologyOperator::DILATE)
    dst_image = src_image->dilate(/*isAlpha*/true);

  inBuffer->setImage(dst_image);
}

void
CSVGFeMorphology::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feMorphology";

    CSVGObject::printValues(os);

    printNameValue(os, "class", class_);
    printNameValue(os, "style", style_);
    printNameValue(os, "in"   , filterIn_);

    if (operator_.isValid()) {
      CSVGMorphologyOperator op = getOperator();

      if      (op == CSVGMorphologyOperator::ERODE ) os << " operator=\"erode\"";
      else if (op == CSVGMorphologyOperator::DILATE) os << " operator=\"dilate\"";
    }

    printNameValue(os, "radius_", radius_);

    os << "/>" << std::endl;
  }
  else
    os << "feMorphology ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeMorphology &filter)
{
  filter.print(os, false);

  return os;
}
