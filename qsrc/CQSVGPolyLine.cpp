#include <CQSVGPolyLine.h>
#include <CQSVG.h>

CQSVGPolyLine::
CQSVGPolyLine(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPolyLine(*svg)
{
}

void
CQSVGPolyLine::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}

void
CQSVGPolyLine::
drawTerm()
{
  drawSelected();
}
