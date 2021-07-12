#ifndef CQSVGMask_H
#define CQSVGMask_H

#include <CQSVGObject.h>
#include <CSVGMask.h>

class CQSVG;

class CQSVGMask : public CQSVGObject, public CSVGMask {
  Q_OBJECT

  Q_PROPERTY(double x      READ getX      WRITE setX     )
  Q_PROPERTY(double y      READ getY      WRITE setY     )
  Q_PROPERTY(double width  READ getWidth  WRITE setWidth )
  Q_PROPERTY(double height READ getHeight WRITE setHeight)

 public:
  CQSVGMask(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
