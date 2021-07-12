#include <CQSVGFeMerge.h>
#include <CQSVG.h>

CQSVGFeMerge::
CQSVGFeMerge(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeMerge(*svg)
{
}

void
CQSVGFeMerge::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}
