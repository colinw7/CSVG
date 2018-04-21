#include <CQSVGFeBlend.h>
#include <CQSVG.h>

CQSVGFeBlend::
CQSVGFeBlend(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeBlend(*svg)
{
}
