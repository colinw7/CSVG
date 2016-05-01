#include <CQSVGTextPath.h>
#include <CQSVG.h>

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
  return CSVGTextPath::getStartOffset().value();
}

void
CQSVGTextPath::
setStartOffset(double r)
{
  CSVGTextPath::setStartOffset(r);
}

void
CQSVGTextPath::
drawTerm()
{
  drawSelected();
}
