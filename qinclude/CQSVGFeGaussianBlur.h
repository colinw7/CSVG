#ifndef CQSVGFeGaussianBlur_H
#define CQSVGFeGaussianBlur_H

#include <CQSVGObject.h>
#include <CSVGFeGaussianBlur.h>

class CQSVG;

class CQSVGFeGaussianBlur : public CQSVGObject, public CSVGFeGaussianBlur {
  Q_OBJECT

  Q_PROPERTY(double stdDevX READ getStdDevX WRITE setStdDevX)
  Q_PROPERTY(double stdDevY READ getStdDevY WRITE setStdDevY)

 public:
  CQSVGFeGaussianBlur(CQSVG *svg);
};

#endif
