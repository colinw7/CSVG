#include <CQSVGStop.h>
#include <CQSVG.h>

CQSVGStop::
CQSVGStop(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGStop(*svg)
{
}

double
CQSVGStop::
getOffset() const
{
  return CSVGStop::getOffset().px(1).value();
}
