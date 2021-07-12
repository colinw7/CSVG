#ifndef CQSVGAnimateTransform_H
#define CQSVGAnimateTransform_H

#include <CQSVGAnimateBase.h>
#include <CSVGAnimateTransform.h>

class CQSVG;

class CQSVGAnimateTransform : public CQSVGAnimateBase, public CSVGAnimateTransform {
  Q_OBJECT

 public:
  CQSVGAnimateTransform(CQSVG *svg);

  void addProperties(CQPropertyTree *propTree, const std::string &name) override;
};

#endif
