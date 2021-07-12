#include <CQSVGTSpan.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGTSpan::
CQSVGTSpan(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGTSpan(*svg)
{
}

void
CQSVGTSpan::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x"   );
  propTree->addProperty(objName, this, "y"   );
  propTree->addProperty(objName, this, "text");
}

void
CQSVGTSpan::
drawTerm()
{
  drawSelected();
}
