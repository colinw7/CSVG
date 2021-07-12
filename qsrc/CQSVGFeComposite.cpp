#include <CQSVGFeComposite.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeComposite::
CQSVGFeComposite(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeComposite(*svg)
{
}

void
CQSVGFeComposite::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "k1");
  propTree->addProperty(objName, this, "k2");
  propTree->addProperty(objName, this, "k3");
  propTree->addProperty(objName, this, "k4");
}
