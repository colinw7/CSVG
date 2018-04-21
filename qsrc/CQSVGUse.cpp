#include <CQSVGUse.h>
#include <CQSVG.h>

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
drawTerm()
{
  drawSelected();
}
