#ifndef CQSVGFilter_H
#define CQSVGFilter_H

#include <CQSVGObject.h>
#include <CSVGFilter.h>

class CQSVG;

class CQSVGFilter : public CQSVGObject, public CSVGFilter {
  Q_OBJECT

  Q_PROPERTY(double x      READ getX      WRITE setX     )
  Q_PROPERTY(double y      READ getY      WRITE setY     )
  Q_PROPERTY(double width  READ getWidth  WRITE setWidth )
  Q_PROPERTY(double height READ getHeight WRITE setHeight)

 public:
  CQSVGFilter(CQSVG *svg);

  double getX() const;
  double getY() const;
  double getWidth() const;
  double getHeight() const;

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
