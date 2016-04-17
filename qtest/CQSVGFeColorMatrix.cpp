#include <CQSVGFeColorMatrix.h>
#include <CQSVG.h>

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
