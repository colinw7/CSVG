#include <CQSVGDefs.h>
#include <CQSVG.h>

CQSVGDefs::
CQSVGDefs(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGDefs(*svg)
{
}

void
CQSVGDefs::
drawTerm()
{
  drawSelected();
}
