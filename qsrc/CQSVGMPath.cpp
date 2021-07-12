#include <CQSVGMPath.h>
#include <CQSVG.h>

CQSVGMPath::
CQSVGMPath(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGMPath(*svg)
{
}

void
CQSVGMPath::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}
