#ifndef CQSVGAnimateColor_H
#define CQSVGAnimateColor_H

#include <CQSVGObject.h>
#include <CSVGAnimateColor.h>

class CQSVG;

class CQSVGAnimateColor : public CQSVGObject, public CSVGAnimateColor {
  Q_OBJECT

 public:
  CQSVGAnimateColor(CQSVG *svg);
};

#endif
