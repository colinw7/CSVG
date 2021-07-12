#ifndef CQSVGFeMerge_H
#define CQSVGFeMerge_H

#include <CQSVGObject.h>
#include <CSVGFeMerge.h>

class CQSVG;

class CQSVGFeMerge : public CQSVGObject, public CSVGFeMerge {
  Q_OBJECT

 public:
  CQSVGFeMerge(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
