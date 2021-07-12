#include <CQSVGFeFlood.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeFlood::
CQSVGFeFlood(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeFlood(*svg)
{
}

void
CQSVGFeFlood::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "opacity");
}
