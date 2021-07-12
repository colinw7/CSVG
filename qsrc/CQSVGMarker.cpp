#include <CQSVGMarker.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGMarker::
CQSVGMarker(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGMarker(*svg)
{
}

void
CQSVGMarker::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "refX");
  propTree->addProperty(objName, this, "refY");
}
