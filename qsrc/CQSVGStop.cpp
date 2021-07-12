#include <CQSVGStop.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGStop::
CQSVGStop(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGStop(*svg)
{
}

double
CQSVGStop::
getOffset() const
{
  return CSVGStop::getOffset().px(1).value();
}

void
CQSVGStop::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "offset" );
  propTree->addProperty(objName, this, "opacity");
}
