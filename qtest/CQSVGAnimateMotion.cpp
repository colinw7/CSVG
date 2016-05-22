#include <CQSVGAnimateMotion.h>
#include <CQSVG.h>

CQSVGAnimateMotion::
CQSVGAnimateMotion(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGAnimateMotion(*svg)
{
}
