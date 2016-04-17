#include <CQSVGPolygon.h>
#include <CQSVG.h>

CQSVGPolygon::
CQSVGPolygon(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPolygon(*svg)
{
}

void
CQSVGPolygon::
drawTerm()
{
  drawSelected();
}
