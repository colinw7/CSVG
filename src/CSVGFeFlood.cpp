#include <CSVGFeFlood.h>
#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

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
 color_    (fe.color_),
 opacity_  (fe.opacity_)
{
}

CSVGFeFlood *
CSVGFeFlood::
dup() const
{
  return new CSVGFeFlood(*this);
}

std::string
CSVGFeFlood::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeFlood::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
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
    color_ = svg_.nameToColor(str);
  else if (svg_.realOption(opt_name, opt_value, "flood-opacity", &real))
    opacity_ = real;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeFlood::
drawElement()
{
  auto *outBuffer = svg_.getBuffer(getFilterOut());

  //---

  CBBox2D bbox;

  auto *filter = getParentFilter();

  if (filter && filter->lastElement()) {
    filter->lastElement()->getSubRegion(bbox);
    //std::cerr << "flood last sub region: " << bbox << std::endl;
  }
  else {
    getSubRegion(bbox);
    //std::cerr << "flood element sub region: " << bbox << std::endl;
  }

  //---

  CRGBA c(0, 0, 0, 0);

  if (color_.isValid())
    c = color_.getValue().rgba();

  if (opacity_.isValid())
    c.setAlpha(opacity_.getValue());

  CSVGBuffer::floodBuffers(c, bbox, outBuffer);

  outBuffer->setBBox(bbox);

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    auto *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
    buffer->setBBox       (bbox);
  }

  return true;
}

void
CSVGFeFlood::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feFlood";

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "feFlood ";
}

void
CSVGFeFlood::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue(os, "in"    , filterIn_ );
  printNameValue(os, "result", filterOut_);

  if (color_.isValid())
    os << " flood-color=\"" << color_.getValue() << "\"";

  printNameValue(os, "flood-opacity", opacity_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeFlood &fe)
{
  fe.print(os, false);

  return os;
}
