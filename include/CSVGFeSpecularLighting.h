#ifndef CSVGFeSpecularLighting_H
#define CSVGFeSpecularLighting_H

#include <CSVGFilterBase.h>
#include <CPoint3D.h>

class CSVGFePointLight;
class CSVGBuffer;

class CSVGFeSpecularLighting : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feSpecularLighting", CSVGObjTypeId::FE_SPECULAR_LIGHTING)

  CSVGFeSpecularLighting(CSVG &svg);
  CSVGFeSpecularLighting(const CSVGFeSpecularLighting &fe);

  CSVGFeSpecularLighting *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("SourceGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("SourceGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  CRGBA getLightingColor() const { return lightingColor_.getValue(CRGBA(1,1,1)); }
  void setLightingColor(const CRGBA &c) { lightingColor_ = c; }

  double getSpecularConstant() const { return specularConstant_.getValue(1); }
  void setSpecularConstant(double r) { specularConstant_ = r; }

  double getSpecularExponent() const { return specularConstant_.getValue(1); }
  void setSpecularExponent(double r) { specularExponent_ = r; }

  double getSurfaceScale() const { return specularConstant_.getValue(1); }
  void setSurfaceScale(double r) { surfaceScale_ = r; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeSpecularLighting &fe);

 private:
  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

  void pointLight(CImagePtr image, CSVGFePointLight *pl);

  CRGBA lightPoint(CRGBA &rgba, const CPoint3D &point) const;

 private:
  COptValT<std::string> filterIn_;
  COptValT<std::string> filterOut_;
  COptValT<CRGBA>       lightingColor_;
  COptValT<double>      specularConstant_;
  COptValT<double>      specularExponent_;
  COptValT<double>      surfaceScale_;

  mutable CPoint3D lpoint_;
  mutable CRGBA    lcolor_;
  mutable double   specConstant_;
  mutable double   specExponent_;
};

#endif
