#include <CQSVGPattern.h>
#include <CQSVG.h>

CQSVGPattern::
CQSVGPattern(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPattern(*svg)
{
}
