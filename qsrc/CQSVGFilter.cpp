#include <CQSVGFilter.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFilter::
CQSVGFilter(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFilter(*svg)
{
}

double
CQSVGFilter::
getX() const
{
  return CSVGFilter::getX(0).pxValue(1);
}

double
CQSVGFilter::
getY() const
{
  return CSVGFilter::getY(0).pxValue(1);
}

double
CQSVGFilter::
getWidth() const
{
  return CSVGFilter::getWidth(0).pxValue(1);
}

double
CQSVGFilter::
getHeight() const
{
  return CSVGFilter::getHeight(0).pxValue(1);
}

void
CQSVGFilter::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x");
  propTree->addProperty(objName, this, "y");
  propTree->addProperty(objName, this, "width");
  propTree->addProperty(objName, this, "height");
}
