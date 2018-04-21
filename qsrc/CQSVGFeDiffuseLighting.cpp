#include <CQSVGFeDiffuseLighting.h>
#include <CQUtil.h>
#include <CQSVG.h>

CQSVGFeDiffuseLighting::
CQSVGFeDiffuseLighting(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeDiffuseLighting(*svg)
{
}

QColor
CQSVGFeDiffuseLighting::
getLightingColor() const
{
  return CQUtil::toQColor(CSVGFeDiffuseLighting::getLightingColor().rgba());
}

void
CQSVGFeDiffuseLighting::
setLightingColor(const QColor &c)
{
  CSVGColor color(CQUtil::colorToRGBA(c));

  CSVGFeDiffuseLighting::setLightingColor(color);
}
