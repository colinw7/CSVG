#ifndef CQSVGLine_H
#define CQSVGLine_H

#include <CQSVGObject.h>
#include <CSVGLine.h>

class CQSVG;

class CQSVGLine : public CQSVGObject, public CSVGLine {
  Q_OBJECT

  Q_PROPERTY(double x1 READ getX1 WRITE setX1)
  Q_PROPERTY(double y1 READ getY1 WRITE setY1)
  Q_PROPERTY(double x2 READ getX2 WRITE setX2)
  Q_PROPERTY(double y2 READ getY2 WRITE setY2)

 public:
  CQSVGLine(CQSVG *svg);

  double getX1() const;
  double getY1() const;
  double getX2() const;
  double getY2() const;

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
