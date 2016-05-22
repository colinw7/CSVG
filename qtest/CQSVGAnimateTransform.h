#ifndef CQSVGAnimateTransform_H
#define CQSVGAnimateTransform_H

#include <CQSVGObject.h>
#include <CSVGAnimateTransform.h>

class CQSVG;

class CQSVGAnimateTransform : public CQSVGObject, public CSVGAnimateTransform {
  Q_OBJECT

 public:
  CQSVGAnimateTransform(CQSVG *svg);
};

#endif
