#ifndef CSVGFeLighting_H
#define CSVGFeLighting_H

#include <CSVGFilterBase.h>
#include <CSVGLightData.h>

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

  std::string getFilterIn() const { return filterIn_.getValue("SourceGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("SourceGraphic"); }
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

 protected:
  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

  void distantLight(CImagePtr image, CSVGFeDistantLight *pl);
  void pointLight  (CImagePtr image, CSVGFePointLight *pl);
  void spotLight   (CImagePtr image, CSVGFeSpotLight *pl);

  CRGBA lightPoint(CImagePtr image, int x, int y) const;

 protected:
  COptValT<std::string> filterIn_;
  COptValT<std::string> filterOut_;
  COptValT<CRGBA>       lightingColor_;
  COptValT<double>      diffuseConstant_;
  COptValT<double>      specularConstant_;
  COptValT<double>      specularExponent_;
  COptValT<double>      surfaceScale_;
  mutable CSVGLightData lightData_;
};

#endif
