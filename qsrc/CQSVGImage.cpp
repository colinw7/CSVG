#include <CQSVGImage.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGImage::
CQSVGImage(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGImage(*svg)
{
}

QString
CQSVGImage::
getXLink() const
{
  return CSVGImage::getLinkName().c_str();
}

void
CQSVGImage::
setXLink(const QString &s)
{
  CSVGImage::setLinkName(s.toStdString());
}

void
CQSVGImage::
drawTerm()
{
  drawSelected();
}

void
CQSVGImage::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "x"     );
  propTree->addProperty(objName, this, "y"     );
  propTree->addProperty(objName, this, "width" );
  propTree->addProperty(objName, this, "height");
}
