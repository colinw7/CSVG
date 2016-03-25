#ifndef CSVGMask_H
#define CSVGMask_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CSVGMask : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("mask", CSVGObjTypeId::MASK)

  CSVGMask(CSVG &svg);
  CSVGMask(const CSVGMask &mask);

  CSVGMask *dup() const override;

  bool getUnitsValid() const { return units_.isValid(); }
  CSVGCoordUnits getUnits() const { return units_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void objectDraw(const CSVGObject &object);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMask &mask);

 private:
  CSVGMask &operator=(const CSVGMask &rhs);

 private:
  CSVGObject *             object_ { 0 };
  COptValT<double>         x_;
  COptValT<double>         y_;
  COptValT<double>         width_;
  COptValT<double>         height_;
  COptValT<CSVGCoordUnits> units_;
};

#endif
