#ifndef CQSVGFeImage_H
#define CQSVGFeImage_H

#include <CQSVGObject.h>
#include <CSVGFeImage.h>

class CQSVG;

class CQSVGFeImage : public CQSVGObject, public CSVGFeImage {
  Q_OBJECT

  Q_PROPERTY(double x      READ getX      WRITE setX)
  Q_PROPERTY(double y      READ getY      WRITE setY)
  Q_PROPERTY(double width  READ getWidth  WRITE setWidth)
  Q_PROPERTY(double height READ getHeight WRITE setHeight)

 public:
  CQSVGFeImage(CQSVG *svg);
};

#endif
