#include <CQSVGFeMorphology.h>
#include <CQSVG.h>

CQSVGFeMorphology::
CQSVGFeMorphology(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeMorphology(*svg)
{
}
