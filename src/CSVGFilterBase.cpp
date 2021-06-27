#include <CSVGFilterBase.h>
#include <CSVGFilter.h>
#include <CSVGBuffer.h>
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
    filterUnits_ = units;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "primitiveUnits", units))
    primitiveUnits_ = units; // TODO: needed ?
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
getFilterObjectBBox(CBBox2D &bbox) const
{
  CSVGFilter *filter = getParentFilter();

  if (! filter)
    return false;

  CSVGObject *obj = filter->getObject();

  bbox = filter->getObjectBBox(obj);

  return true;
}

bool
CSVGFilterBase::
getFilterRegion(CBBox2D &bbox) const
{
  CSVGFilter *filter = getParentFilter();

  if (! filter)
    return false;

  CSVGObject *obj = filter->getObject();

  if (! obj)
    return false;

  if (! filter->getRegion(obj, bbox))
    return false;

  return true;
}

bool
CSVGFilterBase::
getBufferSubRegion(CSVGBuffer *inBuffer, CBBox2D &bbox) const
{
  // get input buffer region
  CBBox2D bufferBBox = inBuffer->bbox();

  if (! bufferBBox.isSet())
    return getSubRegion(bbox);

  // get parent object region
  CBBox2D objectBBox;

  if (! getFilterObjectBBox(objectBBox)) {
    objectBBox = bufferBBox;
  }

  return getBBoxSubRegion(bufferBBox, objectBBox, bbox);
}

bool
CSVGFilterBase::
getSubRegion(CBBox2D &bbox) const
{
  // TODO: use inbuffer and/or last element for reference bbox

  // get parent filter region
  CBBox2D filterBBox;

  if (! getFilterRegion(filterBBox)) {
    CBBox2D viewBBox;

    if (! getParentViewBox(viewBBox))
      viewBBox = CBBox2D(0, 0, 1, 1);

    filterBBox = viewBBox;
  }

  // get parent object region
  CBBox2D objectBBox;

  if (! getFilterObjectBBox(objectBBox)) {
    objectBBox = filterBBox;
  }

  return getBBoxSubRegion(filterBBox, objectBBox, bbox);
}

bool
CSVGFilterBase::
getBBoxSubRegion(const CBBox2D &inBBox, const CBBox2D &objectBBox, CBBox2D &bbox) const
{
  // calc filter primitive subregion
  // x, y, width, height are in primitive units of parent filter
  // TODO: default is tightest fitting bounding box of filter primitives
  double x = inBBox.getXMin  ();
  double y = inBBox.getYMin  ();
  double w = inBBox.getWidth ();
  double h = inBBox.getHeight();

  CSVGCoordUnits primitiveUnits = getParentFilter()->getPrimitiveUnits();

  //---

  if (hasX()) {
    double x1 = getX().pxValue(CScreenUnits(1));

    if (primitiveUnits == CSVGCoordUnits::OBJECT_BBOX)
      x1 = CMathUtil::map(x1, 0, 1, objectBBox.getXMin(), objectBBox.getXMax());

    x = x1;
  }

  if (hasY()) {
    double y1 = getY().pxValue(1);

    if (primitiveUnits == CSVGCoordUnits::OBJECT_BBOX)
      y1 = CMathUtil::map(y1, 0, 1, objectBBox.getYMin(), objectBBox.getYMax());

    y = y1;
  }

  //---

  if (hasWidth()) {
    double w1 = getWidth().pxValue(1);

    if (primitiveUnits == CSVGCoordUnits::OBJECT_BBOX)
      w1 *= objectBBox.getWidth();

    w = w1;
  }

  if (hasHeight()) {
    double h1 = getHeight().pxValue(1);

    if (primitiveUnits == CSVGCoordUnits::OBJECT_BBOX)
      h1 *= objectBBox.getHeight();

    h = h1;
  }

  //---

  bbox = CBBox2D(x, y, x + w, y + h);

  return true;
}

CSVGObject *
CSVGFilterBase::
getParentFilterObject() const
{
  CSVGFilter *filter = getParentFilter();

  if (! filter)
    return nullptr;

  return filter->getObject();
}

CSVGFilter *
CSVGFilterBase::
getParentFilter() const
{
  CSVGFilter *filter = nullptr;

  CSVGObject *parent = getParent();

  while (parent) {
    filter = dynamic_cast<CSVGFilter *>(parent);

    if (filter)
      return filter;

    parent = parent->getParent();
  }

  return nullptr;
}

bool
CSVGFilterBase::
draw()
{
  bool rc = drawElement();

  auto *filter = getParentFilter();

  filter->setLastElement(this);

  return rc;
}

bool
CSVGFilterBase::
getBBox(CBBox2D &bbox) const
{
  return getFilterRegion(bbox);
}

void
CSVGFilterBase::
printValues(std::ostream &os, bool /*flat*/) const
{
  printNameCoordUnits(os, "filterUnits"   , filterUnits_);
  printNameCoordUnits(os, "primitiveUnits", primitiveUnits_);
  printNameLength    (os, "x"             , x_);
  printNameLength    (os, "y"             , y_);
  printNameLength    (os, "width"         , width_);
  printNameLength    (os, "height"        , height_);
  printNameValue     (os, "filterRes"     , filterRes_);

  if (! xlink_.getValue().isNull())
    printNameXLink(os, "xlink:href", xlink_);
}
