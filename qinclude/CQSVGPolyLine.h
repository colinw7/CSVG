#ifndef CQSVGPolyLine_H
#define CQSVGPolyLine_H

#include <CQSVGObject.h>
#include <CSVGPolyLine.h>

class CQSVG;

class CQSVGPolyLine : public CQSVGObject, public CSVGPolyLine {
  Q_OBJECT

 public:
  CQSVGPolyLine(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
