#include <CQSVGAnimateColor.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGAnimateColor::
CQSVGAnimateColor(CQSVG *svg) :
 CQSVGAnimateBase(svg, this), CSVGAnimateColor(*svg)
{
}

QString
CQSVGAnimateColor::
type() const
{
  return CSVGAnimateColor::getType().c_str();
}

QString
CQSVGAnimateColor::
additive() const
{
  return CSVGAnimateColor::getAdditive().c_str();
}

void
CQSVGAnimateColor::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGAnimateBase::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "type");
  propTree->addProperty(objName, this, "additive");
}
