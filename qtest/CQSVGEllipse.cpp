#include <CQSVGEllipse.h>
#include <CQSVG.h>

CQSVGEllipse::
CQSVGEllipse(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGEllipse(*svg)
{
}

void
CQSVGEllipse::
drawTerm()
{
  drawSelected();
}
