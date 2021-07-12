#include <CQSVGFePointLight.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFePointLight::
CQSVGFePointLight(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFePointLight(*svg)
{
}

void
CQSVGFePointLight::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x");
  propTree->addProperty(objName, this, "y");
  propTree->addProperty(objName, this, "z");
}
