#ifndef CQSVGFeSpotLight_H
#define CQSVGFeSpotLight_H

#include <CQSVGObject.h>
#include <CSVGFeSpotLight.h>

class CQSVG;

class CQSVGFeSpotLight : public CQSVGObject, public CSVGFeSpotLight {
  Q_OBJECT

 public:
  CQSVGFeSpotLight(CQSVG *svg);
};

#endif
