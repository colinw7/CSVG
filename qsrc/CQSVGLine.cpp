#include <CQSVGLine.h>
#include <CQSVG.h>

CQSVGLine::
CQSVGLine(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGLine(*svg)
{
}

double
CQSVGLine::
getX1() const
{
  return CSVGLine::getX1().pxValue(1);
}

double
CQSVGLine::
getY1() const
{
  return CSVGLine::getY1().pxValue(1);
}

double
CQSVGLine::
getX2() const
{
  return CSVGLine::getX2().pxValue(1);
}

double
CQSVGLine::
getY2() const
{
  return CSVGLine::getY2().pxValue(1);
}

void
CQSVGLine::
drawTerm()
{
  drawSelected();
}
