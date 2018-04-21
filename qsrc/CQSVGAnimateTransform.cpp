#include <CQSVGAnimateTransform.h>
#include <CQSVG.h>

CQSVGAnimateTransform::
CQSVGAnimateTransform(CQSVG *svg) :
 CQSVGAnimateBase(svg, this), CSVGAnimateTransform(*svg)
{
}
