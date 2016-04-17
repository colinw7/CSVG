#ifndef CQSVGImage_H
#define CQSVGImage_H

#include <CQSVGObject.h>
#include <CSVGImage.h>

class CQSVG;

class CQSVGImage : public CQSVGObject, public CSVGImage {
  Q_OBJECT

  Q_PROPERTY(double x      READ getX      WRITE setX     )
  Q_PROPERTY(double y      READ getY      WRITE setY     )
  Q_PROPERTY(double width  READ getWidth  WRITE setWidth )
  Q_PROPERTY(double height READ getHeight WRITE setHeight)

 public:
  CQSVGImage(CQSVG *svg);

  void drawTerm() override;
};

#endif
