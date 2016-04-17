#include <CQSVGTSpan.h>
#include <CQSVG.h>

CQSVGTSpan::
CQSVGTSpan(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGTSpan(*svg)
{
}

void
CQSVGTSpan::
drawTerm()
{
  drawSelected();
}
