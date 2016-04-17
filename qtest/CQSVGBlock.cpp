#include <CQSVGBlock.h>
#include <CQSVG.h>

CQSVGBlock::
CQSVGBlock(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGBlock(*svg)
{
}

void
CQSVGBlock::
drawTerm()
{
  drawSelected();
}
