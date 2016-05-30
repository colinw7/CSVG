#include <CQSVGAnimateColor.h>
#include <CQSVG.h>

CQSVGAnimateColor::
CQSVGAnimateColor(CQSVG *svg) :
 CQSVGAnimateBase(svg, this), CSVGAnimateColor(*svg)
{
}

QString
CQSVGAnimateColor::
type() const
{
  return CSVGAnimateColor::getType().c_str();
}

QString
CQSVGAnimateColor::
additive() const
{
  return CSVGAnimateColor::getAdditive().c_str();
}
