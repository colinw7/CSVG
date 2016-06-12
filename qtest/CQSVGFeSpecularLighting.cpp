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
  return CQUtil::toQColor(CSVGFeSpecularLighting::getLightingColor().rgba());
}

void
CQSVGFeSpecularLighting::
setLightingColor(const QColor &c)
{
  CSVGColor color(CQUtil::colorToRGBA(c));

  CSVGFeSpecularLighting::setLightingColor(color);
}
