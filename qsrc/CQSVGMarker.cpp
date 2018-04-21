#include <CQSVGMarker.h>
#include <CQSVG.h>

CQSVGMarker::
CQSVGMarker(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGMarker(*svg)
{
}
