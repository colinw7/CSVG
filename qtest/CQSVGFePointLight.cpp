#include <CQSVGFePointLight.h>
#include <CQSVG.h>

CQSVGFePointLight::
CQSVGFePointLight(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFePointLight(*svg)
{
}
