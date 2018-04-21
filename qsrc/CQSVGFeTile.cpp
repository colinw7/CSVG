#include <CQSVGFeTile.h>
#include <CQSVG.h>

CQSVGFeTile::
CQSVGFeTile(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeTile(*svg)
{
}
