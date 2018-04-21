#include <CQSVGFeDisplacementMap.h>
#include <CQSVG.h>

CQSVGFeDisplacementMap::
CQSVGFeDisplacementMap(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeDisplacementMap(*svg)
{
}
