#include <CQSVGTitle.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGTitle::
CQSVGTitle(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGTitle(*svg)
{
}

void
CQSVGTitle::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "text");
}

void
CQSVGTitle::
drawTerm()
{
  drawSelected();
}
