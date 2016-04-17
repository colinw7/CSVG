#include <CQSVGFeOffset.h>
#include <CQSVG.h>

CQSVGFeOffset::
CQSVGFeOffset(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeOffset(*svg)
{
}
