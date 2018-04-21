#include <CQSVGCircle.h>
#include <CQSVG.h>

CQSVGCircle::
CQSVGCircle(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGCircle(*svg)
{
}

double
CQSVGCircle::
getCenterX() const
{
  return CSVGCircle::getCenterX().pxValue(1);
}

double
CQSVGCircle::
getCenterY() const
{
  return CSVGCircle::getCenterY().pxValue(1);
}

double
CQSVGCircle::
getRadius() const
{
  return CSVGCircle::getRadius().pxValue(1);
}

void
CQSVGCircle::
drawTerm()
{
  drawSelected();
}
