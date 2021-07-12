#include <CQSVGLine.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

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
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x1");
  propTree->addProperty(objName, this, "y1");
  propTree->addProperty(objName, this, "x2");
  propTree->addProperty(objName, this, "y2");
}

void
CQSVGLine::
drawTerm()
{
  drawSelected();
}
