#include <CQSVGFeDiffuseLighting.h>
#include <CQUtil.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeDiffuseLighting::
CQSVGFeDiffuseLighting(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeDiffuseLighting(*svg)
{
}

QColor
CQSVGFeDiffuseLighting::
getLightingColor() const
{
  return CQUtil::toQColor(CSVGFeDiffuseLighting::getLightingColor().getValue().rgba());
}

void
CQSVGFeDiffuseLighting::
setLightingColor(const QColor &c)
{
  CSVGColor color(CQUtil::colorToRGBA(c));

  CSVGFeDiffuseLighting::setLightingColor(Color(color));
}

void
CQSVGFeDiffuseLighting::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "lightingColor");
  propTree->addProperty(objName, this, "surfaceScale");
  propTree->addProperty(objName, this, "diffuseConstant");
}
