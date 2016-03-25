#include <CSVGFilter.h>
#include <CSVG.h>

CSVGFilter::
CSVGFilter(CSVG &svg) :
 CSVGObject(svg),
 object_   (0)
{
}

CSVGFilter::
CSVGFilter(const CSVGFilter &filter) :
 CSVGObject(filter),
 object_   (0)
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
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "filterUnits", str))
    units_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "primitiveUnits", str))
    primitiveUnits_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "x", str))
    x_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "y", str))
    y_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "width", str))
    width_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "height", str))
    height_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "filterRes", str))
    filterRes_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "color-interpolation-filters", str))
    colorInterFilters_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = str;
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
  svg_.setBufferImage("SourceGraphic", image);

  for (const auto &c : children()) {
    CSVGFilter *filter = dynamic_cast<CSVGFilter *>(c);

    if (filter)
      filter->draw();
  }

  image = svg_.getBufferImage("SourceGraphic");

  return image;
}

void
CSVGFilter::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<filter";

    printNameValue(os, "id", id_);

    printNameValue(os, "filterUnits"                , units_);
    printNameValue(os, "primitiveUnits"             , primitiveUnits_);
    printNameValue(os, "x"                          , x_);
    printNameValue(os, "y"                          , y_);
    printNameValue(os, "width"                      , width_);
    printNameValue(os, "height"                     , height_);
    printNameValue(os, "filterRes"                  , filterRes_);
    printNameValue(os, "color-interpolation-filters", colorInterFilters_);
    printNameValue(os, "xlink:href"                 , xlink_);

    printTransform(os);

    os << ">" << std::endl;

    for (const auto &o : objects_)
      o->print(os, hier);

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
