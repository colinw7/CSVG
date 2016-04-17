#ifndef CQSVGEllipse_H
#define CQSVGEllipse_H

#include <CQSVGObject.h>
#include <CSVGEllipse.h>

class CQSVG;

class CQSVGEllipse : public CQSVGObject, public CSVGEllipse {
  Q_OBJECT

  Q_PROPERTY(double cx READ getCenterX WRITE setCenterX)
  Q_PROPERTY(double cy READ getCenterY WRITE setCenterY)
  Q_PROPERTY(double rx READ getRadiusX WRITE setRadiusX)
  Q_PROPERTY(double ry READ getRadiusY WRITE setRadiusY)

 public:
  CQSVGEllipse(CQSVG *svg);

  void drawTerm() override;
};

#endif
