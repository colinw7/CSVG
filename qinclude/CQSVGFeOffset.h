#ifndef CQSVGFeOffset_H
#define CQSVGFeOffset_H

#include <CQSVGObject.h>
#include <CSVGFeOffset.h>

class CQSVG;

class CQSVGFeOffset : public CQSVGObject, public CSVGFeOffset {
  Q_OBJECT

  Q_PROPERTY(double dx READ getDX WRITE setDX)
  Q_PROPERTY(double dy READ getDY WRITE setDY)

 public:
  CQSVGFeOffset(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
