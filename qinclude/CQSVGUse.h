#ifndef CQSVGUse_H
#define CQSVGUse_H

#include <CQSVGObject.h>
#include <CSVGUse.h>

class CQSVG;

class CQSVGUse : public CQSVGObject, public CSVGUse {
  Q_OBJECT

  Q_PROPERTY(double  x      READ getX      WRITE setX     )
  Q_PROPERTY(double  y      READ getY      WRITE setY     )
  Q_PROPERTY(double  width  READ getWidth  WRITE setWidth )
  Q_PROPERTY(double  height READ getHeight WRITE setHeight)
  Q_PROPERTY(QString xlink  READ getXLink  WRITE setXLink )

 public:
  CQSVGUse(CQSVG *svg);

  QString getXLink() const;
  void setXLink(const QString &s);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
