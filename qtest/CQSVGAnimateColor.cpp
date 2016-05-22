#include <CQSVGAnimateColor.h>
#include <CQSVG.h>

CQSVGAnimateColor::
CQSVGAnimateColor(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGAnimateColor(*svg)
{
}
