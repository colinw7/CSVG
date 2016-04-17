#ifndef CQSVGFePointLight_H
#define CQSVGFePointLight_H

#include <CQSVGObject.h>
#include <CSVGFePointLight.h>

class CQSVG;

class CQSVGFePointLight : public CQSVGObject, public CSVGFePointLight {
  Q_OBJECT

  Q_PROPERTY(double x READ getX WRITE setX)
  Q_PROPERTY(double y READ getY WRITE setY)
  Q_PROPERTY(double z READ getZ WRITE setZ)

 public:
  CQSVGFePointLight(CQSVG *svg);
};

#endif
