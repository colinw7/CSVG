#include <CQSVGText.h>
#include <CQSVG.h>

CQSVGText::
CQSVGText(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGText(*svg)
{
}

void
CQSVGText::
drawTerm()
{
  drawSelected();
}
