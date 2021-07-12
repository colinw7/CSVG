#ifndef CQSVGSet_H
#define CQSVGSet_H

#include <CQSVGAnimateBase.h>
#include <CSVGSet.h>

class CQSVG;

class CQSVGSet : public CQSVGAnimateBase, public CSVGSet {
  Q_OBJECT

 public:
  CQSVGSet(CQSVG *svg);

  void addProperties(CQPropertyTree *propTree, const std::string &name) override;
};

#endif
