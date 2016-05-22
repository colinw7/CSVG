#ifndef CQSVGAnimateMotion_H
#define CQSVGAnimateMotion_H

#include <CQSVGObject.h>
#include <CSVGAnimateMotion.h>

class CQSVG;

class CQSVGAnimateMotion : public CQSVGObject, public CSVGAnimateMotion {
  Q_OBJECT

 public:
  CQSVGAnimateMotion(CQSVG *svg);
};

#endif
