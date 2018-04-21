#include <CQSVGPath.h>
#include <CQSVG.h>

CQSVGPath::
CQSVGPath(CQSVG *svg) :
 CQSVGObject(svg, this), CSVGPath(*svg)
{
}

QString
CQSVGPath::
getPathString() const
{
  std::stringstream ss;

  ss << CSVGPath::getPartList();

  return ss.str().c_str();
}

void
CQSVGPath::
setPathString(const QString &s)
{
  CSVGPathPartList parts;

  if (! qsvg_->pathStringToParts(s.toStdString(), parts))
    return;

  CSVGPath::setPartList(parts);
}

void
CQSVGPath::
drawTerm()
{
  drawSelected();
}
