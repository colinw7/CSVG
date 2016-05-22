#include <CQSVGAnimateTransform.h>
#include <CQSVG.h>

CQSVGAnimateTransform::
CQSVGAnimateTransform(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGAnimateTransform(*svg)
{
}
