#include <CQSVGAnimateMotion.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGAnimateMotion::
CQSVGAnimateMotion(CQSVG *svg) :
 CQSVGAnimateBase(svg, this), CSVGAnimateMotion(*svg)
{
}

QString
CQSVGAnimateMotion::
pathStr() const
{
  std::stringstream ss;

  ss << CSVGAnimateMotion::getPath();

  return ss.str().c_str();
}

QString
CQSVGAnimateMotion::
keyPoints() const
{
  return CSVGAnimateMotion::getKeyPoints().c_str();
}

void
CQSVGAnimateMotion::
setKeyPoints(const QString &s)
{
  CSVGAnimateMotion::setKeyPoints(s.toStdString());
}

QString
CQSVGAnimateMotion::
rotate() const
{
  return CSVGAnimateMotion::getRotate().c_str();
}

void
CQSVGAnimateMotion::
setRotate(const QString &s)
{
  CSVGAnimateMotion::setRotate(s.toStdString());
}

QString
CQSVGAnimateMotion::
origin() const
{
  return CSVGAnimateMotion::getOrigin().c_str();
}

void
CQSVGAnimateMotion::
setOrigin(const QString &s)
{
  CSVGAnimateMotion::setOrigin(s.toStdString());
}

void
CQSVGAnimateMotion::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGAnimateBase::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "pathStr");
  propTree->addProperty(objName, this, "keyPoints");
  propTree->addProperty(objName, this, "rotate");
  propTree->addProperty(objName, this, "origin");
}
