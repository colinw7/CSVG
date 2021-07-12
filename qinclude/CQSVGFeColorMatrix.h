#ifndef CQSVGFeColorMatrix_H
#define CQSVGFeColorMatrix_H

#include <CQSVGObject.h>
#include <CSVGFeColorMatrix.h>

class CQSVG;

class CQSVGFeColorMatrix : public CQSVGObject, public CSVGFeColorMatrix {
  Q_OBJECT

  Q_PROPERTY(CQSVGEnum::ColorMatrixType type READ getType WRITE setType)

 public:
  CQSVGFeColorMatrix(CQSVG *svg);

  CQSVGEnum::ColorMatrixType getType() const;
  void setType(const CQSVGEnum::ColorMatrixType &t);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
