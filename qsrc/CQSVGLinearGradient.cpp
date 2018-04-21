#include <CQSVGLinearGradient.h>
#include <CQSVG.h>

CQSVGLinearGradient::
CQSVGLinearGradient(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGLinearGradient(*svg)
{
}

double
CQSVGLinearGradient::
getX1() const
{
  return CSVGLinearGradient::getX1().pxValue(1);
}

double
CQSVGLinearGradient::
getY1() const
{
  return CSVGLinearGradient::getY1().pxValue(1);
}

double
CQSVGLinearGradient::
getX2() const
{
  return CSVGLinearGradient::getX2().pxValue(1);
}

double
CQSVGLinearGradient::
getY2() const
{
  return CSVGLinearGradient::getY2().pxValue(1);
}
