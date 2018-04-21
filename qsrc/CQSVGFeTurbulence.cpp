#include <CQSVGFeTurbulence.h>
#include <CQSVG.h>

CQSVGFeTurbulence::
CQSVGFeTurbulence(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeTurbulence(*svg)
{
}
