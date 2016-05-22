#include <CQSVGRadialGradient.h>
#include <CQSVG.h>

CQSVGRadialGradient::
CQSVGRadialGradient(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGRadialGradient(*svg)
{
}

double
CQSVGRadialGradient::
getCenterX() const
{
  return CSVGRadialGradient::getCenterX().pxValue(1);
}

double
CQSVGRadialGradient::
getCenterY() const
{
  return CSVGRadialGradient::getCenterY().pxValue(1);
}

double
CQSVGRadialGradient::
getRadius() const
{
  return CSVGRadialGradient::getRadius().pxValue(1);
}
