#ifndef CQSVGClipPath_H
#define CQSVGClipPath_H

#include <CQSVGObject.h>
#include <CSVGClipPath.h>

class CQSVG;

class CQSVGClipPath : public CQSVGObject, public CSVGClipPath {
  Q_OBJECT

 public:
  CQSVGClipPath(CQSVG *svg);

  void drawTerm() override;
};

#endif
