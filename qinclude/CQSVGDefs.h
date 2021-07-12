#ifndef CQSVGDefs_H
#define CQSVGDefs_H

#include <CQSVGObject.h>
#include <CSVGDefs.h>

class CQSVG;

class CQSVGDefs : public CQSVGObject, public CSVGDefs {
  Q_OBJECT

 public:
  CQSVGDefs(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
