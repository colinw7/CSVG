#include <CQSVGMPath.h>
#include <CQSVG.h>

CQSVGMPath::
CQSVGMPath(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGMPath(*svg)
{
}
