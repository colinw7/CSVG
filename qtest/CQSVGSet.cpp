#include <CQSVGSet.h>
#include <CQSVG.h>

CQSVGSet::
CQSVGSet(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGSet(*svg)
{
}
