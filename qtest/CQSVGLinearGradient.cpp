#include <CQSVGLinearGradient.h>
#include <CQSVG.h>

CQSVGLinearGradient::
CQSVGLinearGradient(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGLinearGradient(*svg)
{
}
