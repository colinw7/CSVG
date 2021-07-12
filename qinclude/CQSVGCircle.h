#ifndef CQSVGCircle_H
#define CQSVGCircle_H

#include <CQSVGObject.h>
#include <CSVGCircle.h>

class CQSVG;

class CQSVGCircle : public CQSVGObject, public CSVGCircle {
  Q_OBJECT

  Q_PROPERTY(double cx READ getCenterX WRITE setCenterX)
  Q_PROPERTY(double cy READ getCenterY WRITE setCenterY)
  Q_PROPERTY(double r  READ getRadius  WRITE setRadius )

 public:
  CQSVGCircle(CQSVG *svg);

  double getCenterX() const;
  double getCenterY() const;
  double getRadius () const;

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
