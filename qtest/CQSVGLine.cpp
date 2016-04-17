#include <CQSVGLine.h>
#include <CQSVG.h>

CQSVGLine::
CQSVGLine(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGLine(*svg)
{
}

void
CQSVGLine::
drawTerm()
{
  drawSelected();
}
