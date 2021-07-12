#include <CQSVGFeImage.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

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

void
CQSVGFeImage::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "width" );
  propTree->addProperty(objName, this, "height");
}
