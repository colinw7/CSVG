#include <CQSVGAnchor.h>
#include <CQSVG.h>

CQSVGAnchor::
CQSVGAnchor(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGAnchor(*svg)
{
}
