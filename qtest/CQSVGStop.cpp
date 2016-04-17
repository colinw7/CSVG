#include <CQSVGStop.h>
#include <CQSVG.h>

CQSVGStop::
CQSVGStop(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGStop(*svg)
{
}
