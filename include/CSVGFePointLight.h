#ifndef CSVGFePointLight_H
#define CSVGFePointLight_H

#include <CSVGFilterBase.h>
#include <CPoint3D.h>

class CSVGFePointLight : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("fePointLight", CSVGObjTypeId::FE_POINT_LIGHT)

  CSVGFePointLight(CSVG &svg);
  CSVGFePointLight(const CSVGFePointLight &fe);

  CSVGFePointLight *dup() const override;

  double getX() const { return x_.getValue(0); }
  void setX(double r) { x_ = r; }

  double getY() const { return y_.getValue(0); }
  void setY(double r) { y_ = r; }

  double getZ() const { return z_.getValue(0); }
  void setZ(double r) { z_ = r; }

  CPoint3D getPoint() const { return CPoint3D(getX(), getY(), getZ()); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFePointLight &fe);

 private:
  COptReal x_;
  COptReal y_;
  COptReal z_;
};

#endif
