#include <CQSVGFeImage.h>
#include <CQSVG.h>

CQSVGFeImage::
CQSVGFeImage(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeImage(*svg)
{
}

double
CQSVGFeImage::
getX() const
{
  return CSVGFeImage::getX().pxValue(1);
}

double
CQSVGFeImage::
getY() const
{
  return CSVGFeImage::getY().pxValue(1);
}

double
CQSVGFeImage::
getWidth() const
{
  return CSVGFeImage::getWidth().pxValue(1);
}

double
CQSVGFeImage::
getHeight() const
{
  return CSVGFeImage::getHeight().pxValue(1);
}
