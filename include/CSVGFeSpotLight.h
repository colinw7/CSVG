#ifndef CSVGFeSpotLight_H
#define CSVGFeSpotLight_H

#include <CSVGFilterBase.h>
#include <CPoint3D.h>

class CSVGFeSpotLight : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feSpotLight", CSVGObjTypeId::FE_SPOT_LIGHT)

  CSVGFeSpotLight(CSVG &svg);
  CSVGFeSpotLight(const CSVGFeSpotLight &fe);

  CSVGFeSpotLight *dup() const override;

  double getX() const { return x_.getValue(0); }
  void setX(double r) { x_ = r; }

  double getY() const { return y_.getValue(0); }
  void setY(double r) { y_ = r; }

  double getZ() const { return z_.getValue(0); }
  void setZ(double r) { z_ = r; }

  CPoint3D getPoint() const { return CPoint3D(getX(), getY(), getZ()); }

  double getPointsAtX() const { return pointsAtX_.getValue(0); }
  void setPointsAtX(double r) { pointsAtX_ = r; }

  double getPointsAtY() const { return pointsAtY_.getValue(0); }
  void setPointsAtY(double r) { pointsAtY_ = r; }

  double getPointsAtZ() const { return pointsAtZ_.getValue(0); }
  void setPointsAtZ(double r) { pointsAtZ_ = r; }

  CPoint3D getPointsAt() const { return CPoint3D(getPointsAtX(), getPointsAtY(), getPointsAtZ()); }

  double getSpecularExponent() const { return specularExponent_.getValue(0); }
  void setSpecularExponent(double r) { specularExponent_ = r; }

  bool hasLimitingConeAngle() const { return limitingConeAngle_.isValid(); }
  double getLimitingConeAngle() const { return limitingConeAngle_.getValue(0); }
  void setLimitingConeAngle(double r) { limitingConeAngle_ = r; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeSpotLight &fe);

 private:
  COptReal x_;
  COptReal y_;
  COptReal z_;
  COptReal pointsAtX_;
  COptReal pointsAtY_;
  COptReal pointsAtZ_;
  COptReal specularExponent_;
  COptReal limitingConeAngle_;
};

#endif
