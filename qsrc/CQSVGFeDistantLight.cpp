#include <CQSVGFeDistantLight.h>
#include <CQSVG.h>

CQSVGFeDistantLight::
CQSVGFeDistantLight(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeDistantLight(*svg)
{
}
