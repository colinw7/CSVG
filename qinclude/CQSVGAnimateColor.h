#ifndef CQSVGAnimateColor_H
#define CQSVGAnimateColor_H

#include <CQSVGAnimateBase.h>
#include <CSVGAnimateColor.h>

class CQSVG;

class CQSVGAnimateColor : public CQSVGAnimateBase, public CSVGAnimateColor {
  Q_OBJECT

  Q_PROPERTY(QString type     READ type)
  Q_PROPERTY(QString additive READ additive)

 public:
  CQSVGAnimateColor(CQSVG *svg);

  QString type() const;

  QString additive() const;

  void addProperties(CQPropertyTree *propTree, const std::string &name) override;
};

#endif
