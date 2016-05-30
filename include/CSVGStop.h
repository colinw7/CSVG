#ifndef CSVGStop_H
#define CSVGStop_H

#include <CSVGObject.h>

class CSVGStop : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("stop", CSVGObjTypeId::STOP)

  CSVGStop(CSVG &svg);
  CSVGStop(const CSVGStop &stop);

  CSVGStop *dup() const override;

  CScreenUnits getOffset () const { return offset_ .getValue(CScreenUnits(0)); }
  CRGBA        getColor  () const { return color_  .getValue(CRGBA(1,1,1)); }
  double       getOpacity() const { return opacity_.getValue(1); }

  CRGBA getAlphaColor() const;

  void setOffset (double offset     ) { offset_  = offset ; }
  void setColor  (const CRGBA &color) { color_   = color  ; }
  void setOpacity(double opacity    ) { opacity_ = opacity; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGStop &stop);

 private:
  COptValT<CScreenUnits> offset_;
  COptValT<CRGBA>        color_;
  COptValT<double>       opacity_;
};

#endif
