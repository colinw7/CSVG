#include <CQSVGFeTile.h>
#include <CQSVG.h>

CQSVGFeTile::
CQSVGFeTile(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeTile(*svg)
{
}

void
CQSVGFeTile::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);
}
