#include <CQSVGFeSpotLight.h>
#include <CQSVG.h>

CQSVGFeSpotLight::
CQSVGFeSpotLight(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeSpotLight(*svg)
{
}
