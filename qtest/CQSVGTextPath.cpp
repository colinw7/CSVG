#include <CQSVGTextPath.h>
#include <CQSVG.h>

CQSVGTextPath::
CQSVGTextPath(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGTextPath(*svg)
{
}

void
CQSVGTextPath::
drawTerm()
{
  drawSelected();
}
