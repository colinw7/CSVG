#include <CQSVGRadialGradient.h>
#include <CQSVG.h>

CQSVGRadialGradient::
CQSVGRadialGradient(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGRadialGradient(*svg)
{
}
