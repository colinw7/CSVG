#ifndef CQSVGPath_H
#define CQSVGPath_H

#include <CQSVGObject.h>
#include <CSVGPath.h>

class CQSVG;

class CQSVGPath : public CQSVGObject, public CSVGPath {
  Q_OBJECT

  Q_PROPERTY(QString pathString READ getPathString WRITE setPathString)
  Q_PROPERTY(double  pathLength READ getPathLength WRITE setPathLength)

 public:
  CQSVGPath(CQSVG *svg);

  QString getPathString() const;
  void setPathString(const QString &s);

  void drawTerm() override;
};

#endif
