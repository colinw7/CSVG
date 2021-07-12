#include <CQSVGPattern.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGPattern::
CQSVGPattern(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPattern(*svg)
{
}

double
CQSVGPattern::
getX() const
{
  return CSVGPattern::getX().pxValue(1);
}

double
CQSVGPattern::
getY() const
{
  return CSVGPattern::getY().pxValue(1);
}

double
CQSVGPattern::
getWidth() const
{
  return CSVGPattern::getWidth().pxValue(1);
}

double
CQSVGPattern::
getHeight() const
{
  return CSVGPattern::getHeight().pxValue(1);
}

CQSVGEnum::CoordUnitsType
CQSVGPattern::
getUnits() const
{
  return CQSVGEnum::coordUnitsTypeConv(CSVGPattern::getUnits());
}

void
CQSVGPattern::
setUnits(const CQSVGEnum::CoordUnitsType &u)
{
  CSVGPattern::setUnits(CQSVGEnum::coordUnitsTypeConv(u));
}

CQSVGEnum::CoordUnitsType
CQSVGPattern::
getContentUnits() const
{
  return CQSVGEnum::coordUnitsTypeConv(CSVGPattern::getContentUnits());
}

void
CQSVGPattern::
setContentUnits(const CQSVGEnum::CoordUnitsType &u)
{
  CSVGPattern::setContentUnits(CQSVGEnum::coordUnitsTypeConv(u));
}

void
CQSVGPattern::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x"           );
  propTree->addProperty(objName, this, "y"           );
  propTree->addProperty(objName, this, "width"       );
  propTree->addProperty(objName, this, "height"      );
  propTree->addProperty(objName, this, "units"       );
  propTree->addProperty(objName, this, "contentUnits");
}
