#include <CQSVGRadialGradient.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGRadialGradient::
CQSVGRadialGradient(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGRadialGradient(*svg)
{
}

double
CQSVGRadialGradient::
getCenterX() const
{
  return CSVGRadialGradient::getCenterX().pxValue(1);
}

double
CQSVGRadialGradient::
getCenterY() const
{
  return CSVGRadialGradient::getCenterY().pxValue(1);
}

double
CQSVGRadialGradient::
getRadius() const
{
  return CSVGRadialGradient::getRadius().pxValue(1);
}

void
CQSVGRadialGradient::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "cx");
  propTree->addProperty(objName, this, "cy");
  propTree->addProperty(objName, this, "r" );
  propTree->addProperty(objName, this, "fx");
  propTree->addProperty(objName, this, "fy");
}
