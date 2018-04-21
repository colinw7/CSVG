#include <CQSVGTitle.h>
#include <CQSVG.h>

CQSVGTitle::
CQSVGTitle(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGTitle(*svg)
{
}

void
CQSVGTitle::
drawTerm()
{
  drawSelected();
}
