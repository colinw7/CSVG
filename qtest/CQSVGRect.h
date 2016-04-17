#ifndef CQSVGRect_H
#define CQSVGRect_H

#include <CQSVGObject.h>
#include <CSVGRect.h>

class CQSVG;

class CQSVGRect : public CQSVGObject, public CSVGRect {
  Q_OBJECT

  Q_PROPERTY(double x  READ getX       WRITE setX      )
  Q_PROPERTY(double y  READ getY       WRITE setY      )
  Q_PROPERTY(double w  READ getWidth   WRITE setWidth  )
  Q_PROPERTY(double h  READ getHeight  WRITE setHeight )
  Q_PROPERTY(double rx READ getRX      WRITE setRX     )
  Q_PROPERTY(double ry READ getRY      WRITE setRY     )

 public:
  CQSVGRect(CQSVG *svg);

  void drawTerm() override;
};

#endif
