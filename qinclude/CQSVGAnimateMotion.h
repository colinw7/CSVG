#ifndef CQSVGAnimateMotion_H
#define CQSVGAnimateMotion_H

#include <CQSVGAnimateBase.h>
#include <CSVGAnimateMotion.h>

class CQSVG;

class CQSVGAnimateMotion : public CQSVGAnimateBase, public CSVGAnimateMotion {
  Q_OBJECT

  Q_PROPERTY(QString pathStr   READ pathStr)
  Q_PROPERTY(QString keyPoints READ keyPoints WRITE setKeyPoints)
  Q_PROPERTY(QString rotate    READ rotate    WRITE setRotate)
  Q_PROPERTY(QString origin    READ origin    WRITE setOrigin)

 public:
  CQSVGAnimateMotion(CQSVG *svg);

  QString pathStr() const;

  QString keyPoints() const;
  void setKeyPoints(const QString &s);

  QString rotate() const;
  void setRotate(const QString &s);

  QString origin() const;
  void setOrigin(const QString &s);

  void addProperties(CQPropertyTree *propTree, const std::string &name) override;
};

#endif
