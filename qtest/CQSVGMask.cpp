#include <CQSVGMask.h>
#include <CQSVG.h>

CQSVGMask::
CQSVGMask(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGMask(*svg)
{
}
