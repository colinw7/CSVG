#include <CQSVGSymbol.h>
#include <CQSVG.h>

CQSVGSymbol::
CQSVGSymbol(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGSymbol(*svg)
{
}
