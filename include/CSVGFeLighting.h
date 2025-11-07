#ifndef CSVGFeLighting_H
#define CSVGFeLighting_H

#include <CSVGFilterBase.h>

class CSVGBuffer;
class CSVGFeDistantLight;
class CSVGFePointLight;
class CSVGFeSpotLight;

class CSVGFeLighting : public CSVGFilterBase, public CSVGPrintBase<CSVGFeLighting> {
 public:
  CSVG_OBJECT_DEF("feLighting", CSVGObjTypeId::FE_SPECULAR_LIGHTING)

  CSVGFeLighting(CSVG &svg);
  CSVGFeLighting(const CSVGFeLighting &fe);

  virtual bool isDiffuse () const { return false; }
  virtual bool isSpecular() const { return false; }

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  Color getLightingColor() const { return lightingColor_.value_or(Color()); }
  void setLightingColor(const Color &c) { lightingColor_ = c; }

  double getDiffuseConstant() const { return diffuseConstant_.value_or(1); }
  void setDiffuseConstant(double r) { diffuseConstant_ = r; }

  double getSpecularConstant() const { return specularConstant_.value_or(1); }
  void setSpecularConstant(double r) { specularConstant_ = r; }

  double getSpecularExponent() const { return specularExponent_.value_or(1); }
  void setSpecularExponent(double r) { specularExponent_ = r; }

  double getSurfaceScale() const { return surfaceScale_.value_or(1); }
  void setSurfaceScale(double r) { surfaceScale_ = r; }

  void filterImage(CSVGBuffer *inBuffer, const CBBox2D &bbox, CSVGBuffer *outBuffer);

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  std::optional<std::string> filterIn_;
  std::optional<std::string> filterOut_;
  std::optional<Color>       lightingColor_;
  std::optional<double>      diffuseConstant_;
  std::optional<double>      specularConstant_;
  std::optional<double>      specularExponent_;
  std::optional<double>      surfaceScale_;
};

#endif
