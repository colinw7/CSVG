#include <CQSVGFeDistantLight.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeDistantLight::
CQSVGFeDistantLight(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeDistantLight(*svg)
{
}

void
CQSVGFeDistantLight::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "elevation");
  propTree->addProperty(objName, this, "azimuth"  );
}
