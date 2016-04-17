#ifndef CQSVGPolygon_H
#define CQSVGPolygon_H

#include <CQSVGObject.h>
#include <CSVGPolygon.h>

class CQSVG;

class CQSVGPolygon : public CQSVGObject, public CSVGPolygon {
  Q_OBJECT

 public:
  CQSVGPolygon(CQSVG *svg);

  void drawTerm() override;
};

#endif
