#include <CQSVGAnimate.h>
#include <CQSVG.h>

CQSVGAnimate::
CQSVGAnimate(CQSVG *svg) :
 CQSVGAnimateBase(svg, this), CSVGAnimate(*svg)
{
}
