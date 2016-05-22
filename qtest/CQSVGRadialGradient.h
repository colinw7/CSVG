#ifndef CQSVGRadialGradient_H
#define CQSVGRadialGradient_H

#include <CQSVGObject.h>
#include <CSVGRadialGradient.h>

class CQSVG;

class CQSVGRadialGradient : public CQSVGObject, public CSVGRadialGradient {
  Q_OBJECT

  Q_PROPERTY(double cx READ getCenterX WRITE setCenterX)
  Q_PROPERTY(double cy READ getCenterY WRITE setCenterY)
  Q_PROPERTY(double r  READ getRadius  WRITE setRadius )
  Q_PROPERTY(double fx READ getFocusX  WRITE setFocusX )
  Q_PROPERTY(double fy READ getFocusY  WRITE setFocusY )

 public:
  CQSVGRadialGradient(CQSVG *svg);

  double getCenterX() const;
  double getCenterY() const;
  double getRadius () const;
};

#endif
