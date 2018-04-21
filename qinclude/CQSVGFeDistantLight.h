#ifndef CQSVGFeDistantLight_H
#define CQSVGFeDistantLight_H

#include <CQSVGObject.h>
#include <CSVGFeDistantLight.h>

class CQSVG;

class CQSVGFeDistantLight : public CQSVGObject, public CSVGFeDistantLight {
  Q_OBJECT

  Q_PROPERTY(double elevation READ getElevation WRITE setElevation)
  Q_PROPERTY(double azimuth   READ getAzimuth   WRITE setAzimuth  )

 public:
  CQSVGFeDistantLight(CQSVG *svg);
};

#endif
