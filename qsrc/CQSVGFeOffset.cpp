#include <CQSVGFeOffset.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeOffset::
CQSVGFeOffset(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeOffset(*svg)
{
}

void
CQSVGFeOffset::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "dx");
  propTree->addProperty(objName, this, "dy");
}
