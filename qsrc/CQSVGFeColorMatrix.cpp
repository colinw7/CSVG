#include <CQSVGFeColorMatrix.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

CQSVGFeColorMatrix::
CQSVGFeColorMatrix(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGFeColorMatrix(*svg)
{
}

CQSVGEnum::ColorMatrixType
CQSVGFeColorMatrix::
getType() const
{
  return CQSVGEnum::colorMatrixTypeConv(CSVGFeColorMatrix::getType());
}

void
CQSVGFeColorMatrix::
setType(const CQSVGEnum::ColorMatrixType &t)
{
  CSVGFeColorMatrix::setType(CQSVGEnum::colorMatrixTypeConv(t));
}

void
CQSVGFeColorMatrix::
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "type");
}
