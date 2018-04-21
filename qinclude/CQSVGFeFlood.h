#ifndef CQSVGFeFlood_H
#define CQSVGFeFlood_H

#include <CQSVGObject.h>
#include <CSVGFeFlood.h>

class CQSVG;

class CQSVGFeFlood : public CQSVGObject, public CSVGFeFlood {
  Q_OBJECT

  Q_PROPERTY(double opacity READ getOpacity WRITE setOpacity)

 public:
  CQSVGFeFlood(CQSVG *svg);
};

#endif
