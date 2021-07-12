#include <CQSVGFeDisplacementMap.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeDisplacementMap::
CQSVGFeDisplacementMap(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeDisplacementMap(*svg)
{
}

void
CQSVGFeDisplacementMap::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "scale");
}
