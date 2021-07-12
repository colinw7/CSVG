#include <CQSVGFeTurbulence.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeTurbulence::
CQSVGFeTurbulence(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeTurbulence(*svg)
{
}

void
CQSVGFeTurbulence::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "fractalNoise");
  propTree->addProperty(objName, this, "baseFreq");
  propTree->addProperty(objName, this, "numOctaves");
  propTree->addProperty(objName, this, "seed");
}
