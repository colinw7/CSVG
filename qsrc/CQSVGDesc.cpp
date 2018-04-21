#include <CQSVGDesc.h>
#include <CQSVG.h>

CQSVGDesc::
CQSVGDesc(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGDesc(*svg)
{
}

void
CQSVGDesc::
drawTerm()
{
  drawSelected();
}
