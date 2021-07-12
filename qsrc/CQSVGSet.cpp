#include <CQSVGSet.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGSet::
CQSVGSet(CQSVG *svg) :
 CQSVGAnimateBase(svg, this), CSVGSet(*svg)
{
}

void
CQSVGSet::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGAnimateBase::addProperties(propTree, name);
}
