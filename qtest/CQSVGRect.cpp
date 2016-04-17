#include <CQSVGRect.h>
#include <CQSVG.h>

CQSVGRect::
CQSVGRect(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGRect(*svg)
{
}

void
CQSVGRect::
drawTerm()
{
  drawSelected();
}
