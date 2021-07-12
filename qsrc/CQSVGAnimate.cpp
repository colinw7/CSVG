#include <CQSVGAnimate.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGAnimate::
CQSVGAnimate(CQSVG *svg) :
 CQSVGAnimateBase(svg, this), CSVGAnimate(*svg)
{
}

void
CQSVGAnimate::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGAnimateBase::addProperties(propTree, name);
}
