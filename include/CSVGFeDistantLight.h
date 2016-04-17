#ifndef CSVGFeDistantLight_H
#define CSVGFeDistantLight_H

#include <CSVGFilterBase.h>

class CSVGFeDistantLight : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feDistantLight", CSVGObjTypeId::FE_DISTANT_LIGHT)

  CSVGFeDistantLight(CSVG &svg);
  CSVGFeDistantLight(const CSVGFeDistantLight &fe);

  CSVGFeDistantLight *dup() const override;

  double getElevation() const { return elevation_.getValue(0); }
  void setElevation(double r) { elevation_ = r; }

  double getAzimuth() const { return azimuth_.getValue(0); }
  void setAzimuth(double r) { azimuth_ = r; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeDistantLight &fe);

 private:
  COptValT<double> elevation_;
  COptValT<double> azimuth_;
};

#endif
