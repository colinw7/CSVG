#include <CQSVGSymbol.h>
#include <CQSVG.h>

CQSVGSymbol::
CQSVGSymbol(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGSymbol(*svg)
{
}

void
CQSVGSymbol::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}
