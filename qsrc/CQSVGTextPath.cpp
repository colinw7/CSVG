#include <CQSVGTextPath.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGTextPath::
CQSVGTextPath(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGTextPath(*svg)
{
}

QString
CQSVGTextPath::
getXLink() const
{
  return CSVGTextPath::getLinkName().c_str();
}

void
CQSVGTextPath::
setXLink(const QString &s)
{
  CSVGTextPath::setLinkName(s.toStdString());
}

double
CQSVGTextPath::
getStartOffset() const
{
  return CSVGTextPath::getStartOffset().px().value();
}

void
CQSVGTextPath::
setStartOffset(double r)
{
  CSVGTextPath::setStartOffset(r);
}

void
CQSVGTextPath::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "text"       );
  propTree->addProperty(objName, this, "xlink"      );
  propTree->addProperty(objName, this, "startOffset");
}

void
CQSVGTextPath::
drawTerm()
{
  drawSelected();
}
