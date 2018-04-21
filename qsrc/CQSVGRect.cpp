#include <CQSVGRect.h>
#include <CQSVG.h>

CQSVGRect::
CQSVGRect(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGRect(*svg)
{
}

double
CQSVGRect::
getX() const
{
  return CSVGRect::getX().pxValue(CScreenUnits(1));
}

double
CQSVGRect::
getY() const
{
  return CSVGRect::getY().pxValue(CScreenUnits(1));
}

double
CQSVGRect::
getWidth() const
{
  return CSVGRect::getWidth().pxValue(CScreenUnits(1));
}

double
CQSVGRect::
getHeight() const
{
  return CSVGRect::getHeight().pxValue(CScreenUnits(1));
}

void
CQSVGRect::
drawTerm()
{
  drawSelected();
}
