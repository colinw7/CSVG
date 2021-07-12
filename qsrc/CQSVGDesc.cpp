#include <CQSVGDesc.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGDesc::
CQSVGDesc(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGDesc(*svg)
{
}

void
CQSVGDesc::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "text");
}

void
CQSVGDesc::
drawTerm()
{
  drawSelected();
}
