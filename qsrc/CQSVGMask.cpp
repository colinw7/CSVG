#include <CQSVGMask.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGMask::
CQSVGMask(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGMask(*svg)
{
}

void
CQSVGMask::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x"     );
  propTree->addProperty(objName, this, "y"     );
  propTree->addProperty(objName, this, "width" );
  propTree->addProperty(objName, this, "height");
}
