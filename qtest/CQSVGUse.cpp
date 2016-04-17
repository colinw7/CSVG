#include <CQSVGUse.h>
#include <CQSVG.h>

CQSVGUse::
CQSVGUse(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGUse(*svg)
{
}

void
CQSVGUse::
drawTerm()
{
  drawSelected();
}
