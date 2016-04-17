#ifndef CQSVGFeGaussianBlur_H
#define CQSVGFeGaussianBlur_H

#include <CQSVGObject.h>
#include <CSVGFeGaussianBlur.h>

class CQSVG;

class CQSVGFeGaussianBlur : public CQSVGObject, public CSVGFeGaussianBlur {
  Q_OBJECT

  Q_PROPERTY(double stdDev READ getStdDev WRITE setStdDev)

 public:
  CQSVGFeGaussianBlur(CQSVG *svg);
};

#endif
