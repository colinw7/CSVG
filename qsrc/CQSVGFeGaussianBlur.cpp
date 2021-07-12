#include <CQSVGFeGaussianBlur.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeGaussianBlur::
CQSVGFeGaussianBlur(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeGaussianBlur(*svg)
{
}

void
CQSVGFeGaussianBlur::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "stdDevX");
  propTree->addProperty(objName, this, "stdDevY");
}
