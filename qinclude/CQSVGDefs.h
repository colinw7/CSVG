#ifndef CQSVGDefs_H
#define CQSVGDefs_H

#include <CQSVGObject.h>
#include <CSVGDefs.h>

class CQSVG;

class CQSVGDefs : public CQSVGObject, public CSVGDefs {
  Q_OBJECT

 public:
  CQSVGDefs(CQSVG *svg);

  void drawTerm() override;
};

#endif
