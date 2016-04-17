#include <CQSVGCircle.h>
#include <CQSVG.h>

CQSVGCircle::
CQSVGCircle(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGCircle(*svg)
{
}

void
CQSVGCircle::
drawTerm()
{
  drawSelected();
}
