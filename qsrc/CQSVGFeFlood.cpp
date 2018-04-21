#include <CQSVGFeFlood.h>
#include <CQSVG.h>

CQSVGFeFlood::
CQSVGFeFlood(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeFlood(*svg)
{
}
