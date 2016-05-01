#include <CQSVGClipPath.h>
#include <CQSVG.h>

CQSVGClipPath::
CQSVGClipPath(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGClipPath(*svg)
{
}

void
CQSVGClipPath::
drawTerm()
{
  drawSelected();
}
