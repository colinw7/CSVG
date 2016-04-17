#ifndef CQSVGMarker_H
#define CQSVGMarker_H

#include <CQSVGObject.h>
#include <CSVGMarker.h>

class CQSVG;

class CQSVGMarker : public CQSVGObject, public CSVGMarker {
  Q_OBJECT

 public:
  CQSVGMarker(CQSVG *svg);
};

#endif
