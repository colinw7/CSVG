#include <CQSVGFeSpotLight.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeSpotLight::
CQSVGFeSpotLight(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeSpotLight(*svg)
{
}

void
CQSVGFeSpotLight::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x");
  propTree->addProperty(objName, this, "y");
  propTree->addProperty(objName, this, "z");
  propTree->addProperty(objName, this, "pointsAtX");
  propTree->addProperty(objName, this, "pointsAtY");
  propTree->addProperty(objName, this, "pointsAtZ");
  propTree->addProperty(objName, this, "specularExponent");
  propTree->addProperty(objName, this, "limitingConeAngle");
}
