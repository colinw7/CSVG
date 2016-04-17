#include <CQSVGFeSpecularLighting.h>
#include <CQUtil.h>
#include <CQSVG.h>

CQSVGFeSpecularLighting::
CQSVGFeSpecularLighting(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeSpecularLighting(*svg)
{
}

QColor
CQSVGFeSpecularLighting::
getLightingColor() const
{
  return CQUtil::toQColor(CSVGFeSpecularLighting::getLightingColor());
}

void
CQSVGFeSpecularLighting::
setLightingColor(const QColor &c)
{
  CSVGFeSpecularLighting::setLightingColor(CQUtil::colorToRGBA(c));
}
