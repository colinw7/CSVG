#include <CQSVGEllipse.h>
#include <CQSVG.h>

CQSVGEllipse::
CQSVGEllipse(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGEllipse(*svg)
{
}

double
CQSVGEllipse::
getCenterX() const
{
  return CSVGEllipse::getCenterX().pxValue(1);
}

double
CQSVGEllipse::
getCenterY() const
{
  return CSVGEllipse::getCenterY().pxValue(1);
}

double
CQSVGEllipse::
getRadiusX() const
{
  return CSVGEllipse::getRadiusX().pxValue(1);
}

double
CQSVGEllipse::
getRadiusY() const
{
  return CSVGEllipse::getRadiusY().pxValue(1);
}

void
CQSVGEllipse::
drawTerm()
{
  drawSelected();
}
