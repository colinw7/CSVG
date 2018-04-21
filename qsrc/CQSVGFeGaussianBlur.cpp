#include <CQSVGFeGaussianBlur.h>
#include <CQSVG.h>

CQSVGFeGaussianBlur::
CQSVGFeGaussianBlur(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeGaussianBlur(*svg)
{
}
