#include <CQSVGPath.h>
#include <CQSVG.h>

CQSVGPath::
CQSVGPath(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPath(*svg)
{
}

void
CQSVGPath::
drawTerm()
{
  drawSelected();
}
