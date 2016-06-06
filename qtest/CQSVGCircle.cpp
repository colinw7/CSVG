#include <CQSVGCircle.h>
#include <CQSVG.h>

CQSVGCircle::
CQSVGCircle(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGCircle(*svg)
{
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
