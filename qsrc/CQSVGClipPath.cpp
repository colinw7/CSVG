#include <CQSVGClipPath.h>
#include <CQSVG.h>

CQSVGClipPath::
CQSVGClipPath(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGClipPath(*svg)
{
}

void
CQSVGClipPath::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}

void
CQSVGClipPath::
drawTerm()
{
  drawSelected();
}
