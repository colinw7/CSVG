#ifndef CSVGFeLighting_H
#define CSVGFeLighting_H

#include <CSVGFilterBase.h>

class CSVGBuffer;
class CSVGFeDistantLight;
class CSVGFePointLight;
class CSVGFeSpotLight;

class CSVGFeLighting : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feLighting", CSVGObjTypeId::FE_SPECULAR_LIGHTING)

  CSVGFeLighting(CSVG &svg);
  CSVGFeLighting(const CSVGFeLighting &fe);

  virtual bool isDiffuse () const { return false; }
  virtual bool isSpecular() const { return false; }

  std::string getFilterIn() const { return filterIn_.getValue("FilterGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("FilterGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  CRGBA getLightingColor() const { return lightingColor_.getValue(CRGBA(1,1,1)); }
  void setLightingColor(const CRGBA &c) { lightingColor_ = c; }

  double getDiffuseConstant() const { return diffuseConstant_.getValue(1); }
  void setDiffuseConstant(double r) { diffuseConstant_ = r; }

  double getSpecularConstant() const { return specularConstant_.getValue(1); }
  void setSpecularConstant(double r) { specularConstant_ = r; }

  double getSpecularExponent() const { return specularExponent_.getValue(1); }
  void setSpecularExponent(double r) { specularExponent_ = r; }

  double getSurfaceScale() const { return surfaceScale_.getValue(1); }
  void setSurfaceScale(double r) { surfaceScale_ = r; }

  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

 protected:
  COptString      filterIn_;
  COptString      filterOut_;
  COptValT<CRGBA> lightingColor_;
  COptReal        diffuseConstant_;
  COptReal        specularConstant_;
  COptReal        specularExponent_;
  COptReal        surfaceScale_;
};

#endif
