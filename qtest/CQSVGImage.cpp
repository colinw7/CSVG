#include <CQSVGImage.h>
#include <CQSVG.h>

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
