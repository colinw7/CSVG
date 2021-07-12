#include <CQSVGEllipse.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

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
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "cx");
  propTree->addProperty(objName, this, "cy");
  propTree->addProperty(objName, this, "rx");
  propTree->addProperty(objName, this, "ry");
}

void
CQSVGEllipse::
drawTerm()
{
  drawSelected();
}
