#ifndef CQSVGFeComposite_H
#define CQSVGFeComposite_H

#include <CQSVGObject.h>
#include <CSVGFeComposite.h>

class CQSVG;

class CQSVGFeComposite : public CQSVGObject, public CSVGFeComposite {
  Q_OBJECT

  Q_PROPERTY(double k1 READ getK1 WRITE setK1)
  Q_PROPERTY(double k2 READ getK2 WRITE setK2)
  Q_PROPERTY(double k3 READ getK3 WRITE setK3)
  Q_PROPERTY(double k4 READ getK4 WRITE setK4)

 public:
  CQSVGFeComposite(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
