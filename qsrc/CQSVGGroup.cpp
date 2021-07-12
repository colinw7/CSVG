#include <CQSVGGroup.h>
#include <CQSVG.h>

CQSVGGroup::
CQSVGGroup(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGGroup(*svg)
{
}

void
CQSVGGroup::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}

void
CQSVGGroup::
drawTerm()
{
  drawSelected();
}
