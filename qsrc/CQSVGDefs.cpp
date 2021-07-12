#include <CQSVGDefs.h>
#include <CQSVG.h>

CQSVGDefs::
CQSVGDefs(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGDefs(*svg)
{
}

void
CQSVGDefs::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}

void
CQSVGDefs::
drawTerm()
{
  drawSelected();
}
