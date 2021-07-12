#ifndef CQSVGGroup_H
#define CQSVGGroup_H

#include <CQSVGObject.h>
#include <CSVGGroup.h>

class CQSVG;

class CQSVGGroup : public CQSVGObject, public CSVGGroup {
  Q_OBJECT

 public:
  CQSVGGroup(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
