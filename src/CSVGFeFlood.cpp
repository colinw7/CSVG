#include <CSVGFeFlood.h>
#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGUtil.h>

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
draw()
{
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  //---

  // get filtered object coords
  double x1 = 0, y1 = 0, x2 = 100, y2 = 100;

  CSVGFilter *filter = getParentFilter();

  if (filter) {
    CBBox2D bbox;

    filter->getObject()->getBBox(bbox);

    if (! bbox.isSet()) {
      double x = getX     ().pxValue(0);
      double y = getY     ().pxValue(0);
      double w = getWidth ().pxValue(100);
      double h = getHeight().pxValue(100);

      bbox = CBBox2D(x, y, x + w, y + h);
    }

    svg_.windowToPixel(bbox.getXMin(), bbox.getYMin(), &x1, &y1);
    svg_.windowToPixel(bbox.getXMax(), bbox.getYMax(), &x2, &y2);
  }

  //---

  CRGBA c(0,0,0,0);

  if (color_.isValid())
    c = color_.getValue().rgba();

  if (opacity_.isValid())
    c.setAlpha(opacity_.getValue());

  CSVGBuffer::floodBuffers(c, x1, y1, x2 - x1, y2 - y1, outBuffer);

  outBuffer->setBBox(CBBox2D(x1, y1, x2, y2));

  //---

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImageBuffer(outBuffer);
  }

  return true;
}

void
CSVGFeFlood::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feFlood";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "in"    , filterIn_ );
    printNameValue(os, "result", filterOut_);

    if (color_.isValid())
      os << " flood-color=\"" << color_.getValue() << "\"";

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
