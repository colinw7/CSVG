#include <CQSVGFeMorphology.h>
#include <CQSVG.h>

CQSVGFeMorphology::
CQSVGFeMorphology(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeMorphology(*svg)
{
}

void
CQSVGFeMorphology::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}
