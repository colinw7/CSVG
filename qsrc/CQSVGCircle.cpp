#include <CQSVGCircle.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

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
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "cx");
  propTree->addProperty(objName, this, "cy");
  propTree->addProperty(objName, this, "r" );
}

void
CQSVGCircle::
drawTerm()
{
  drawSelected();
}
