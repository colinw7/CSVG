#ifndef CQSVGGroup_H
#define CQSVGGroup_H

#include <CQSVGObject.h>
#include <CSVGGroup.h>

class CQSVG;

class CQSVGGroup : public CQSVGObject, public CSVGGroup {
  Q_OBJECT

 public:
  CQSVGGroup(CQSVG *svg);

  void drawTerm() override;
};

#endif
