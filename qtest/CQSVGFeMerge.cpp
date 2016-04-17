#include <CQSVGFeMerge.h>
#include <CQSVG.h>

CQSVGFeMerge::
CQSVGFeMerge(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeMerge(*svg)
{
}
