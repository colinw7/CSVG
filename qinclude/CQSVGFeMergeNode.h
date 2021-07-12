#ifndef CQSVGFeMergeNode_H
#define CQSVGFeMergeNode_H

#include <CQSVGObject.h>
#include <CSVGFeMergeNode.h>

class CQSVG;

class CQSVGFeMergeNode : public CQSVGObject, public CSVGFeMergeNode {
  Q_OBJECT

 public:
  CQSVGFeMergeNode(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
