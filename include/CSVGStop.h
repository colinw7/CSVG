#ifndef CSVGStop_H
#define CSVGStop_H

#include <CSVGObject.h>
#include <CSVGColor.h>

class CSVGStop : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("stop", CSVGObjTypeId::STOP)

  CSVGStop(CSVG &svg);
  CSVGStop(const CSVGStop &stop);

  CSVGStop *dup() const override;

  CScreenUnits getOffset () const { return offset_.getValue(CScreenUnits(0)); }
  void setOffset(double offset) { offset_ = offset ; }

  CSVGColor getColor  () const { return color_.getValue(CSVGColor()); }
  void setColor(const CSVGColor &color) { color_ = color  ; }

  double getOpacity() const { return opacity_.getValue(1); }
  void setOpacity(double opacity) { opacity_ = opacity; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGStop &stop);

 private:
  COptValT<CScreenUnits> offset_;
  COptValT<CSVGColor>    color_;
  COptReal               opacity_;
};

#endif
