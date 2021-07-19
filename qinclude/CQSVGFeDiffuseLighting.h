#ifndef CQSVGFeDiffuseLighting_H
#define CQSVGFeDiffuseLighting_H

#include <CQSVGObject.h>
#include <CSVGFeDiffuseLighting.h>

class CQSVG;

class CQSVGFeDiffuseLighting : public CQSVGObject, public CSVGFeDiffuseLighting {
  Q_OBJECT

  Q_PROPERTY(QColor lightingColor   READ getLightingColor   WRITE setLightingColor  )
  Q_PROPERTY(double surfaceScale    READ getSurfaceScale    WRITE setSurfaceScale   )
  Q_PROPERTY(double diffuseConstant READ getDiffuseConstant WRITE setDiffuseConstant)

 public:
  using Color = CSVGInheritValT<CSVGColor>;

 public:
  CQSVGFeDiffuseLighting(CQSVG *svg);

  QColor getLightingColor() const;
  void setLightingColor(const QColor &c);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
