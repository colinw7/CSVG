#include <CQSVGAnchor.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGAnchor::
CQSVGAnchor(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGAnchor(*svg)
{
}

QString
CQSVGAnchor::
getXLink() const
{
  return CSVGAnchor::getLinkName().c_str();
}

void
CQSVGAnchor::
setXLink(const QString &s)
{
  CSVGAnchor::setLinkName(s.toStdString());
}

void
CQSVGAnchor::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "xlink");
}
