#include <CQSVGPolygon.h>
#include <CQSVG.h>

CQSVGPolygon::
CQSVGPolygon(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPolygon(*svg)
{
}

void
CQSVGPolygon::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}

void
CQSVGPolygon::
drawTerm()
{
  drawSelected();
}
