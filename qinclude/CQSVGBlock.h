#ifndef CQSVGBlock_H
#define CQSVGBlock_H

#include <CQSVGObject.h>
#include <CSVGBlock.h>

class CQSVG;

class CQSVGBlock : public CQSVGObject, public CSVGBlock {
  Q_OBJECT

  Q_PROPERTY(double x READ getX WRITE setX)
  Q_PROPERTY(double y READ getY WRITE setY)

  Q_PROPERTY(double width  READ getWidth  WRITE setWidth )
  Q_PROPERTY(double height READ getHeight WRITE setHeight)

 public:
  CQSVGBlock(CQSVG *svg);

  void drawTerm() override;
};

#endif
