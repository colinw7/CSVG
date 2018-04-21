#ifndef CQSVGFeSpotLight_H
#define CQSVGFeSpotLight_H

#include <CQSVGObject.h>
#include <CSVGFeSpotLight.h>

class CQSVG;

class CQSVGFeSpotLight : public CQSVGObject, public CSVGFeSpotLight {
  Q_OBJECT

  Q_PROPERTY(double x                 READ getX                 WRITE setX                )
  Q_PROPERTY(double y                 READ getY                 WRITE setY                )
  Q_PROPERTY(double z                 READ getZ                 WRITE setZ                )
  Q_PROPERTY(double pointsAtX         READ getPointsAtX         WRITE setPointsAtX        )
  Q_PROPERTY(double pointsAtY         READ getPointsAtY         WRITE setPointsAtY        )
  Q_PROPERTY(double pointsAtZ         READ getPointsAtZ         WRITE setPointsAtZ        )
  Q_PROPERTY(double specularExponent  READ getSpecularExponent  WRITE setSpecularExponent )
  Q_PROPERTY(double limitingConeAngle READ getLimitingConeAngle WRITE setLimitingConeAngle)

 public:
  CQSVGFeSpotLight(CQSVG *svg);
};

#endif
