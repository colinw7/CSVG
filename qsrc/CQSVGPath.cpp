#include <CQSVGPath.h>
#include <CQSVGPathPart.h>
#include <CQSVG.h>
#include <CQPropertyTree.h>

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
addProperties(CQPropertyTree *propTree, const std::string &name)
{
  CQSVGObject::addProperties(propTree, name);

  QString objName = name.c_str();

  propTree->addProperty(objName, this, "pathString");

  QString partsName = objName + "/parts";

  int ind = 0;

  for (const auto &part : getPartList().parts()) {
    auto *qpart = dynamic_cast<CQSVGPathPart *>(part);

    auto *qmoveto   = dynamic_cast<CQSVGPathMoveTo      *>(qpart);
    auto *qrmoveto  = dynamic_cast<CQSVGPathRMoveTo     *>(qpart);
    auto *qlineto   = dynamic_cast<CQSVGPathLineTo      *>(qpart);
    auto *qrlineto  = dynamic_cast<CQSVGPathRLineTo     *>(qpart);
    auto *qhlineto  = dynamic_cast<CQSVGPathHLineTo     *>(qpart);
    auto *qrhlineto = dynamic_cast<CQSVGPathRHLineTo    *>(qpart);
    auto *qvlineto  = dynamic_cast<CQSVGPathVLineTo     *>(qpart);
    auto *qrvlineto = dynamic_cast<CQSVGPathRVLineTo    *>(qpart);
    auto *qarcto    = dynamic_cast<CQSVGPathArcTo       *>(qpart);
    auto *qrarcto   = dynamic_cast<CQSVGPathRArcTo      *>(qpart);
    auto *qbez2to   = dynamic_cast<CQSVGPathBezier2To   *>(qpart);
    auto *qmbez2to  = dynamic_cast<CQSVGPathMBezier2To  *>(qpart);
    auto *qrbez2to  = dynamic_cast<CQSVGPathRBezier2To  *>(qpart);
    auto *qmrbez2to = dynamic_cast<CQSVGPathMRBezier2To *>(qpart);
    auto *qbez3to   = dynamic_cast<CQSVGPathBezier3To   *>(qpart);
    auto *qmbez3to  = dynamic_cast<CQSVGPathMBezier3To  *>(qpart);
    auto *qrbez3to  = dynamic_cast<CQSVGPathRBezier3To  *>(qpart);
    auto *qmrbez3to = dynamic_cast<CQSVGPathMRBezier3To *>(qpart);

    QString typeName = CSVGPathPart::partTypeName(qpart->getType()).c_str();
    QString partName = partsName + "/" + typeName + ":" + QString("%1").arg(ind + 1);

    if      (qmoveto) {
      propTree->addProperty(partName, qmoveto, "x");
      propTree->addProperty(partName, qmoveto, "y");
    }
    else if (qrmoveto) {
      propTree->addProperty(partName, qrmoveto, "dx");
      propTree->addProperty(partName, qrmoveto, "dy");
    }
    else if (qlineto) {
      propTree->addProperty(partName, qlineto, "x");
      propTree->addProperty(partName, qlineto, "y");
    }
    else if (qrlineto) {
      propTree->addProperty(partName, qrlineto, "dx");
      propTree->addProperty(partName, qrlineto, "dy");
    }
    else if (qhlineto) {
      propTree->addProperty(partName, qhlineto, "x");
    }
    else if (qrhlineto) {
      propTree->addProperty(partName, qrhlineto, "dx");
    }
    else if (qvlineto) {
     propTree->addProperty(partName, qvlineto, "y");
    }
    else if (qrvlineto) {
      propTree->addProperty(partName, qrvlineto, "dy");
    }
    else if (qarcto) {
      propTree->addProperty(partName, qarcto, "rx");
      propTree->addProperty(partName, qarcto, "ry");
      propTree->addProperty(partName, qarcto, "xa");
      propTree->addProperty(partName, qarcto, "fa");
      propTree->addProperty(partName, qarcto, "fs");
      propTree->addProperty(partName, qarcto, "x2");
      propTree->addProperty(partName, qarcto, "y2");
    }
    else if (qrarcto) {
      propTree->addProperty(partName, qrarcto, "rx");
      propTree->addProperty(partName, qrarcto, "ry");
      propTree->addProperty(partName, qrarcto, "xa");
      propTree->addProperty(partName, qrarcto, "fa");
      propTree->addProperty(partName, qrarcto, "fs");
      propTree->addProperty(partName, qrarcto, "x2");
      propTree->addProperty(partName, qrarcto, "y2");
    }
    else if (qbez2to) {
      propTree->addProperty(partName, qbez2to, "x1");
      propTree->addProperty(partName, qbez2to, "y1");
      propTree->addProperty(partName, qbez2to, "x2");
      propTree->addProperty(partName, qbez2to, "y2");
    }
    else if (qmbez2to) {
      propTree->addProperty(partName, qmbez2to, "x2");
      propTree->addProperty(partName, qmbez2to, "y2");
    }
    else if (qrbez2to) {
      propTree->addProperty(partName, qrbez2to, "x1");
      propTree->addProperty(partName, qrbez2to, "y1");
      propTree->addProperty(partName, qrbez2to, "x2");
      propTree->addProperty(partName, qrbez2to, "y2");
    }
    else if (qmrbez2to) {
      propTree->addProperty(partName, qmrbez2to, "x2");
      propTree->addProperty(partName, qmrbez2to, "y2");
    }
    else if (qbez3to) {
      propTree->addProperty(partName, qbez3to, "x1");
      propTree->addProperty(partName, qbez3to, "y1");
      propTree->addProperty(partName, qbez3to, "x2");
      propTree->addProperty(partName, qbez3to, "y2");
      propTree->addProperty(partName, qbez3to, "x3");
      propTree->addProperty(partName, qbez3to, "y3");
    }
    else if (qmbez3to) {
      propTree->addProperty(partName, qmbez3to, "x2");
      propTree->addProperty(partName, qmbez3to, "y2");
      propTree->addProperty(partName, qmbez3to, "x3");
      propTree->addProperty(partName, qmbez3to, "y3");
    }
    else if (qrbez3to) {
      propTree->addProperty(partName, qrbez3to, "x1");
      propTree->addProperty(partName, qrbez3to, "y1");
      propTree->addProperty(partName, qrbez3to, "x2");
      propTree->addProperty(partName, qrbez3to, "y2");
      propTree->addProperty(partName, qrbez3to, "x3");
      propTree->addProperty(partName, qrbez3to, "y3");
    }
    else if (qmrbez3to) {
      propTree->addProperty(partName, qmrbez3to, "x2");
      propTree->addProperty(partName, qmrbez3to, "y2");
      propTree->addProperty(partName, qmrbez3to, "x3");
      propTree->addProperty(partName, qmrbez3to, "y3");
    }
    else {
      propTree->addProperty(partName, qpart, "type");
    }

    ++ind;
  }
}

void
CQSVGPath::
drawTerm()
{
  drawSelected();
}
