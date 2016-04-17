#include <CQSVGPolyLine.h>
#include <CQSVG.h>

CQSVGPolyLine::
CQSVGPolyLine(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPolyLine(*svg)
{
}

void
CQSVGPolyLine::
drawTerm()
{
  drawSelected();
}
