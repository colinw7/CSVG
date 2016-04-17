#ifndef CQSVGUse_H
#define CQSVGUse_H

#include <CQSVGObject.h>
#include <CSVGUse.h>

class CQSVG;

class CQSVGUse : public CQSVGObject, public CSVGUse {
  Q_OBJECT

 public:
  CQSVGUse(CQSVG *svg);

  void drawTerm() override;
};

#endif
