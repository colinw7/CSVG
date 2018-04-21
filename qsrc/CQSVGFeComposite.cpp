#include <CQSVGFeComposite.h>
#include <CQSVG.h>

CQSVGFeComposite::
CQSVGFeComposite(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeComposite(*svg)
{
}
