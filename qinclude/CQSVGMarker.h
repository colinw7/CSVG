#ifndef CQSVGMarker_H
#define CQSVGMarker_H

#include <CQSVGObject.h>
#include <CSVGMarker.h>

class CQSVG;

class CQSVGMarker : public CQSVGObject, public CSVGMarker {
  Q_OBJECT

  Q_PROPERTY(double refX READ getRefX WRITE setRefX)
  Q_PROPERTY(double refY READ getRefY WRITE setRefY)

 public:
  CQSVGMarker(CQSVG *svg);
};

#endif
