#include <CQSVGUse.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGUse::
CQSVGUse(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGUse(*svg)
{
}

QString
CQSVGUse::
getXLink() const
{
  return CSVGUse::getLinkName().c_str();
}

void
CQSVGUse::
setXLink(const QString &s)
{
  CSVGUse::setLinkName(s.toStdString());
}

void
CQSVGUse::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x"     );
  propTree->addProperty(objName, this, "y"     );
  propTree->addProperty(objName, this, "width" );
  propTree->addProperty(objName, this, "height");
  propTree->addProperty(objName, this, "xlink" );
}

void
CQSVGUse::
drawTerm()
{
  drawSelected();
}
