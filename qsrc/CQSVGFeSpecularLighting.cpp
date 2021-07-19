#include <CQSVGFeSpecularLighting.h>
#include <CQUtil.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeSpecularLighting::
CQSVGFeSpecularLighting(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeSpecularLighting(*svg)
{
}

QColor
CQSVGFeSpecularLighting::
getLightingColor() const
{
  return CQUtil::toQColor(CSVGFeSpecularLighting::getLightingColor().getValue().rgba());
}

void
CQSVGFeSpecularLighting::
setLightingColor(const QColor &c)
{
  CSVGColor color(CQUtil::colorToRGBA(c));

  CSVGFeSpecularLighting::setLightingColor(Color(color));
}

void
CQSVGFeSpecularLighting::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "lightingColor");
  propTree->addProperty(objName, this, "specularConstant");
  propTree->addProperty(objName, this, "specularExponent");
  propTree->addProperty(objName, this, "surfaceScale");
}
