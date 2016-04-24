#include <CQSVGPattern.h>
#include <CQSVG.h>

CQSVGPattern::
CQSVGPattern(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPattern(*svg)
{
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
