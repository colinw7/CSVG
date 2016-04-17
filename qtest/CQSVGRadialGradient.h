#ifndef CQSVGRadialGradient_H
#define CQSVGRadialGradient_H

#include <CQSVGObject.h>
#include <CSVGRadialGradient.h>

class CQSVG;

class CQSVGRadialGradient : public CQSVGObject, public CSVGRadialGradient {
  Q_OBJECT

 public:
  CQSVGRadialGradient(CQSVG *svg);
};

#endif
