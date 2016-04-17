#ifndef CSVGFeDiffuseLighting_H
#define CSVGFeDiffuseLighting_H

#include <CSVGFilterBase.h>
#include <CPoint3D.h>

class CSVGBuffer;
class CSVGFeDistantLight;
class CSVGFePointLight;
class CSVGFeSpotLight;

class CSVGFeDiffuseLighting : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feDiffuseLighting", CSVGObjTypeId::FE_DIFFUSE_LIGHTING)

  CSVGFeDiffuseLighting(CSVG &svg);
  CSVGFeDiffuseLighting(const CSVGFeDiffuseLighting &fe);

  CSVGFeDiffuseLighting *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("SourceGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("SourceGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  CRGBA getLightingColor() const { return lightingColor_.getValue(CRGBA(1,1,1)); }
  void setLightingColor(const CRGBA &c) { lightingColor_ = c; }

  double getSurfaceScale() const { return surfaceScale_.getValue(1); }
  void setSurfaceScale(double r) { surfaceScale_ = r; }

  double getDiffuseConstant() const { return diffuseConstant_.getValue(1); }
  void setDiffuseConstant(double r) { diffuseConstant_ = r; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeDiffuseLighting &fe);

 private:
  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

  void distantLight(CImagePtr image, CSVGFeDistantLight *pl);
  void pointLight  (CImagePtr image, CSVGFePointLight *pl);
  void spotLight   (CImagePtr image, CSVGFeSpotLight *pl);

  CRGBA lightPoint(CRGBA &rgba, const CPoint3D &point) const;

 private:
  COptValT<std::string> filterIn_;
  COptValT<std::string> filterOut_;
  COptValT<CRGBA>       lightingColor_;
  COptValT<double>      surfaceScale_;
  COptValT<double>      diffuseConstant_;

  mutable CPoint3D lpoint_;
  mutable CRGBA    lcolor_;
};

#endif
