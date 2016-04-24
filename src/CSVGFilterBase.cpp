#include <CSVGFilterBase.h>
#include <CSVGFilter.h>
#include <CSVG.h>

CSVGFilterBase::
CSVGFilterBase(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGFilterBase::
CSVGFilterBase(const CSVGFilterBase &filter) :
 CSVGObject(filter),
 xlink_    (this)
{
}

std::string
CSVGFilterBase::
getUniqueName() const
{
  CSVGFilter *filter = getParentFilter();

  std::string name;

  if (filter)
    name = filter->getUniqueName() + ":";

  name += CSVGObject::getUniqueName();

  return name;
}

void
CSVGFilterBase::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

bool
CSVGFilterBase::
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

bool
CSVGFilterBase::
getParentBBox(CBBox2D &bbox) const
{
  CSVGFilter *filter = getParentFilter();

  if (! filter)
    return false;

  CBBox2D bbox1;

  if (! filter->getObject()->getBBox(bbox1))
  //if (! filter->getObject()->getFlatTransformedBBox(bbox1))
    return false;

  if (! bbox1.isSet())
    return false;

  double x = filter->getX     (bbox1.getXMin  ());
  double y = filter->getY     (bbox1.getYMin  ());
  double w = filter->getWidth (bbox1.getWidth ());
  double h = filter->getHeight(bbox1.getHeight());

  bbox = CBBox2D(x, y, x + w, y + h);

  return true;
}

CSVGFilter *
CSVGFilterBase::
getParentFilter() const
{
  CSVGFilter *filter = 0;

  CSVGObject *parent = getParent();

  while (parent) {
    filter = dynamic_cast<CSVGFilter *>(parent);

    if (filter)
      return filter;

    parent = parent->getParent();
  }

  return 0;
}
