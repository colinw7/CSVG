#include <CQSVGFeBlend.h>
#include <CQSVG.h>

CQSVGFeBlend::
CQSVGFeBlend(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeBlend(*svg)
{
}

void
CQSVGFeBlend::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}
