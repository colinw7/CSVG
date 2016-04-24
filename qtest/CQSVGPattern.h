#ifndef CQSVGPattern_H
#define CQSVGPattern_H

#include <CQSVGObject.h>
#include <CSVGPattern.h>

class CQSVG;

class CQSVGPattern : public CQSVGObject, public CSVGPattern {
  Q_OBJECT

  Q_PROPERTY(double x      READ getX      WRITE setX     )
  Q_PROPERTY(double y      READ getY      WRITE setY     )
  Q_PROPERTY(double width  READ getWidth  WRITE setWidth )
  Q_PROPERTY(double height READ getHeight WRITE setHeight)

  Q_PROPERTY(CQSVGEnum::CoordUnitsType units        READ getUnits        WRITE setUnits       )
  Q_PROPERTY(CQSVGEnum::CoordUnitsType contentUnits READ getContentUnits WRITE setContentUnits)

 public:
  CQSVGPattern(CQSVG *svg);

  CQSVGEnum::CoordUnitsType getUnits() const;
  void setUnits(const CQSVGEnum::CoordUnitsType &u);

  CQSVGEnum::CoordUnitsType getContentUnits() const;
  void setContentUnits(const CQSVGEnum::CoordUnitsType &u);
};

#endif
