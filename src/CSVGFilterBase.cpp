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

std::string
CSVGFilterBase::
calcFilterIn(const COptString &filterIn) const
{
  if (filterIn.isValid())
    return filterIn.getValue();

  CSVGFilter *filter = getParentFilter();

  return (filter ? filter->getLastFilterName() : "FilterGraphic");
}

std::string
CSVGFilterBase::
calcFilterOut(const COptString &filterOut) const
{
  CSVGFilter *filter = getParentFilter();

  std::string name = "FilterGraphic";

  if (filterOut.isValid())
    name = filterOut.getValue();

  if (filter)
    filter->setLastFilterName(name);

  return name;
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
  CBBox2D viewBBox;

  if (! getParentViewBox(viewBBox))
    viewBBox = CBBox2D(0, 0, 100, 100);

  if (! getParentBBox(bbox))
    bbox = viewBBox;

  //---

  CSVGCoordUnits punits = getParentFilter()->getPrimitiveUnits();

  double pw = viewBBox.getWidth ();
  double ph = viewBBox.getHeight();

  if (hasX()) {
    double x = getX().pxValue(pw);

    if (punits == CSVGCoordUnits::USER_SPACE)
      bbox.moveXTo(x);
    else
      bbox.moveXTo(getXMin() + x);
  }

  if (hasY()) {
    double y = getY().pxValue(ph);

    if (punits == CSVGCoordUnits::USER_SPACE)
      bbox.moveYTo(y);
    else
      bbox.moveYTo(getYMin() + y);
  }

  if (hasWidth()) {
    double w = getWidth().pxValue(pw);

    bbox.setWidth(w);
  }

  if (hasHeight()) {
    double h = getHeight().pxValue(ph);

    bbox.setHeight(h);
  }

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
