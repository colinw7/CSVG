#include <CQSVGGroup.h>
#include <CQSVG.h>

CQSVGGroup::
CQSVGGroup(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGGroup(*svg)
{
}

void
CQSVGGroup::
drawTerm()
{
  drawSelected();
}
