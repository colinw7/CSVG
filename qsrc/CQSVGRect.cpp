#include <CQSVGRect.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

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
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x" );
  propTree->addProperty(objName, this, "y" );
  propTree->addProperty(objName, this, "w" );
  propTree->addProperty(objName, this, "h" );
  propTree->addProperty(objName, this, "rx");
  propTree->addProperty(objName, this, "ry");
}

void
CQSVGRect::
drawTerm()
{
  drawSelected();
}
