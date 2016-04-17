#ifndef CSVGFeSpotLight_H
#define CSVGFeSpotLight_H

#include <CSVGFilterBase.h>

class CSVGFeSpotLight : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feSpotLight", CSVGObjTypeId::FE_SPOT_LIGHT)

  CSVGFeSpotLight(CSVG &svg);
  CSVGFeSpotLight(const CSVGFeSpotLight &fe);

  CSVGFeSpotLight *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeSpotLight &fe);

 private:
  COptValT<double> x_;
  COptValT<double> y_;
  COptValT<double> z_;
  COptValT<double> pointsAtX_;
  COptValT<double> pointsAtY_;
  COptValT<double> pointsAtZ_;
  COptValT<double> specularExponent_;
  COptValT<double> limitingConeAngle_;
};

#endif
