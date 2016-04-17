#ifndef CQSVGPath_H
#define CQSVGPath_H

#include <CQSVGObject.h>
#include <CSVGPath.h>

class CQSVG;

class CQSVGPath : public CQSVGObject, public CSVGPath {
  Q_OBJECT

 public:
  CQSVGPath(CQSVG *svg);

  void drawTerm() override;
};

#endif
