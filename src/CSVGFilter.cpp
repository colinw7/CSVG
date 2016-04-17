#include <CSVGFilter.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGFilter::
CSVGFilter(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGFilter::
CSVGFilter(const CSVGFilter &filter) :
 CSVGObject(filter),
 xlink_    (this)
{
}

CSVGFilter *
CSVGFilter::
dup() const
{
  return new CSVGFilter(*this);
}

bool
CSVGFilter::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string     str;
  CSVGCoordUnits  units;
  CSVGLengthValue length;

  if      (svg_.coordUnitsOption(opt_name, opt_value, "filterUnits", units))
    units_ = units;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "primitiveUnits", units))
    primitiveUnits_ = units;
  else if (svg_.coordOption(opt_name, opt_value, "x", length))
    x_ = length;
  else if (svg_.coordOption(opt_name, opt_value, "y", length))
    y_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length;
  else if (svg_.stringOption(opt_name, opt_value, "filterRes", str))
    filterRes_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else if (processFilterColorOption(opt_name, opt_value))
    return true;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFilter::
draw()
{
}

CImagePtr
CSVGFilter::
filterImage(CImagePtr image)
{
  CSVGBuffer *buffer = svg_.getBuffer("SourceGraphic");

  buffer->setImage(image);

  for (const auto &c : children()) {
    CSVGFilter *filter = dynamic_cast<CSVGFilter *>(c);

    if (filter)
      filter->draw();
  }

  return buffer->getImage();
}

void
CSVGFilter::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<filter";

    CSVGObject::printValues(os);

    printNameCoordUnits(os, "filterUnits"   , units_);
    printNameCoordUnits(os, "primitiveUnits", primitiveUnits_);

    printNameLength(os, "x"     , x_);
    printNameLength(os, "y"     , y_);
    printNameLength(os, "width" , width_);
    printNameLength(os, "height", height_);

    printNameValue(os, "filterRes" , filterRes_);
    printNameXLink(os, "xlink:href", xlink_);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</filter>" << std::endl;
  }
  else
    os << "filter ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFilter &filter)
{
  filter.print(os, false);

  return os;
}
