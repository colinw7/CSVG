#ifndef CSVGFePointLight_H
#define CSVGFePointLight_H

#include <CSVGFilterBase.h>
#include <CPoint3D.h>

class CSVGFePointLight : public CSVGFilterBase, public CSVGPrintBase<CSVGFePointLight> {
 public:
  CSVG_OBJECT_DEF("fePointLight", CSVGObjTypeId::FE_POINT_LIGHT)

  CSVGFePointLight(CSVG &svg);
  CSVGFePointLight(const CSVGFePointLight &fe);

  CSVGFePointLight *dup() const override;

  double getX() const { return x_.value_or(0); }
  void setX(double r) { x_ = r; }

  double getY() const { return y_.value_or(0); }
  void setY(double r) { y_ = r; }

  double getZ() const { return z_.value_or(0); }
  void setZ(double r) { z_ = r; }

  CPoint3D getPoint() const { return CPoint3D(getX(), getY(), getZ()); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<double> x_;
  std::optional<double> y_;
  std::optional<double> z_;
};

#endif
