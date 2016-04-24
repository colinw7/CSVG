#include <CQSVGAnchor.h>
#include <CQSVG.h>

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
