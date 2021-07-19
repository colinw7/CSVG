#ifndef CQSVGFeSpecularLighting_H
#define CQSVGFeSpecularLighting_H

#include <CQSVGObject.h>
#include <CSVGFeSpecularLighting.h>

class CQSVG;

class CQSVGFeSpecularLighting : public CQSVGObject, public CSVGFeSpecularLighting {
  Q_OBJECT

  Q_PROPERTY(QColor lightingColor    READ getLightingColor    WRITE setLightingColor   )
  Q_PROPERTY(double specularConstant READ getSpecularConstant WRITE setSpecularConstant)
  Q_PROPERTY(double specularExponent READ getSpecularExponent WRITE setSpecularExponent)
  Q_PROPERTY(double surfaceScale     READ getSurfaceScale     WRITE setSurfaceScale    )

 public:
  using Color = CSVGInheritValT<CSVGColor>;

 public:
  CQSVGFeSpecularLighting(CQSVG *svg);

  QColor getLightingColor() const;
  void setLightingColor(const QColor &c);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
