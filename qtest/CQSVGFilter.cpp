#include <CQSVGFilter.h>
#include <CQSVG.h>

CQSVGFilter::
CQSVGFilter(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFilter(*svg)
{
}
