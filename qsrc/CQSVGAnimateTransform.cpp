#include <CQSVGAnimateTransform.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGAnimateTransform::
CQSVGAnimateTransform(CQSVG *svg) :
 CQSVGAnimateBase(svg, this), CSVGAnimateTransform(*svg)
{
}

void
CQSVGAnimateTransform::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGAnimateBase::addProperties(propTree, name);
}
