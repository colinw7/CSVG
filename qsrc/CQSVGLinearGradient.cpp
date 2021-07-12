#include <CQSVGLinearGradient.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

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

void
CQSVGLinearGradient::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x1");
  propTree->addProperty(objName, this, "y1");
  propTree->addProperty(objName, this, "x2");
  propTree->addProperty(objName, this, "y2");
}
