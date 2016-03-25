#ifndef CSVGPattern_H
#define CSVGPattern_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CSVGPattern : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("pattern", CSVGObjTypeId::PATTERN)

  CSVGPattern(CSVG &svg);
  CSVGPattern(const CSVGPattern &pattern);

  CSVGPattern *dup() const override;

  bool getUnitsValid() const { return units_.isValid(); }
  CSVGCoordUnits getUnits() const { return units_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { units_ = units; }

  bool getContentsUnitsValid() const { return contentUnits_.isValid(); }
  CSVGCoordUnits getContentsUnits() const {
    return contentUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setContentsUnits(CSVGCoordUnits units) { contentUnits_ = units; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void fillRectangle(double x1, double y1, double x2, double y2);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPattern &pattern);

  CImagePtr getImage(double w, double h, double *w1, double *h1);

 private:
  CSVGPattern &operator=(const CSVGPattern &rhs);

 private:
  CSVGObject*              object_ { 0 };
  COptValT<double>         x_;
  COptValT<double>         y_;
  COptValT<double>         width_;
  COptValT<double>         height_;
  COptValT<CSVGCoordUnits> units_;
  COptValT<CSVGCoordUnits> contentUnits_;
};

#endif
