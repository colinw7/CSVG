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
  std::string    str;
  CSVGCoordUnits units;
  CScreenUnits   length;

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

  CSVGObject *obj = filter->getObject();

  if (! filter->getRegion(obj, bbox))
    return false;

  return true;
}

bool
CSVGFilterBase::
getTransformedParentBBox(CBBox2D &bbox) const
{
  if (! getParentBBox(bbox))
    return false;

  CSVGCoordUnits punits = getParentFilter()->getPrimitiveUnits();

  if (hasX()) {
    if (punits == CSVGCoordUnits::USER_SPACE)
      bbox.moveXTo(getX());
    else
      bbox.moveXTo(getXMin() + getX());
  }

  if (hasY()) {
    if (punits == CSVGCoordUnits::USER_SPACE)
      bbox.moveYTo(getY());
    else
      bbox.moveYTo(getYMin() + getY());
  }

  if (hasWidth())
    bbox.setWidth(getWidth());

  if (hasHeight())
    bbox.setHeight(getHeight());

  return true;
}

CSVGObject *
CSVGFilterBase::
getParentFilterObject() const
{
  CSVGFilter *filter = getParentFilter();

  if (! filter)
    return 0;

  return filter->getObject();
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

void
CSVGFilterBase::
printValues(std::ostream &os) const
{
  printNameCoordUnits(os, "filterUnits"   , units_);
  printNameCoordUnits(os, "primitiveUnits", primitiveUnits_);
  printNameLength    (os, "x"             , x_);
  printNameLength    (os, "y"             , y_);
  printNameLength    (os, "width"         , width_);
  printNameLength    (os, "height"        , height_);
  printNameValue     (os, "filterRes"     , filterRes_);
  printNameXLink     (os, "xlink:href"    , xlink_);
}
