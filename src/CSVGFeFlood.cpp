#include <CSVGFeFlood.h>
#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGUtil.h>
#include <CRGBName.h>

CSVGFeFlood::
CSVGFeFlood(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeFlood::
CSVGFeFlood(const CSVGFeFlood &fe) :
 CSVGFilterBase(fe),
 filterIn_ (fe.filterIn_),
 filterOut_(fe.filterOut_),
 color_    (fe.color_)
{
}

CSVGFeFlood *
CSVGFeFlood::
dup() const
{
  return new CSVGFeFlood(*this);
}

bool
CSVGFeFlood::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str; // needed ?
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "flood-color", str))
    color_ = CRGBName::toRGBA(str);
  else if (svg_.realOption(opt_name, opt_value, "flood-opacity", &real)) {
    opacity_ = real;

    CRGBA c = color_.getValue(CRGBA(0,0,0,0));

    c.setAlpha(real);

    color_ = c;
  }
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeFlood::
draw()
{
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  filterImage(outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
  }
}

void
CSVGFeFlood::
filterImage(CSVGBuffer *outBuffer)
{
  // get filtered object size
  double w = 100;
  double h = 100;

  CSVGFilter *filter = getParentFilter();

  if (filter) {
    CBBox2D bbox;

    filter->getObject()->getBBox(bbox);

    if (bbox.isSet()) {
      double pw, ph;

      svg_.lengthToPixel(bbox.getWidth(), bbox.getHeight(), &pw, &ph);

      w = pw;
      h = ph;
    }
  }

  int pw = CSVGUtil::round(w);
  int ph = CSVGUtil::round(h);

  CSVGBuffer::floodBuffers(color_.getValue(CRGBA(0,0,0,0)), pw, ph, outBuffer);
}

void
CSVGFeFlood::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feFlood";

    CSVGObject::printValues(os);

    printNameValue(os, "in"    , filterIn_ );
    printNameValue(os, "result", filterOut_);

    if (color_.isValid())
      os << " flood-color=\"" << color_.getValue().getRGB().stringEncode() << "\"";

    printNameValue(os, "opacity", opacity_);

    os << "/>" << std::endl;
  }
  else
    os << "feFlood ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeFlood &fe)
{
  fe.print(os, false);

  return os;
}
