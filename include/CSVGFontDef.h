#ifndef CSVGFontDef_H
#define CSVGFontDef_H

#include <CSVGInheritVal.h>
#include <CScreenUnits.h>
#include <CFontStyle.h>
#include <COptVal.h>

class CSVG;
class CSVGFontObj;

class CSVGFontDef {
 public:
  using Family   = CSVGInheritValT<std::string>;
  using FontSize = CSVGInheritValT<CScreenUnits>;

 public:
  CSVGFontDef(CSVG &svg);
  CSVGFontDef(const CSVGFontDef &font_def);

  CSVGFontDef &operator=(const CSVGFontDef &font_def);

  bool isSet() const {
    return (family_.isValid() || size_.isValid() || style_.isValid());
  }

  const CSVG &svg() const { return svg_; }

  void reset();

  bool hasFamily() const { return family_.isValid(); }
  Family getFamily() const { return family_.getValue(Family("serif")); }
  void setFamily(const Family &family);

  bool hasSize() const { return size_.isValid(); }
  FontSize getSize() const { return size_.getValue(FontSize(CScreenUnits(12))); }
  void setSize(const FontSize &lvalue);

  bool hasStyle() const { return style_.isValid(); }
  CFontStyles getStyle() const { return style_.getValue(CFontStyles(CFONT_STYLE_NORMAL)); }
  void setStyle(CFontStyles s);

  bool hasAngle() const { return angle_.isValid(); }
  double getAngle() const { return angle_.getValue(0); }
  void setAngle(double a);

  void setStyle (const std::string &styleDef);
  void setWeight(const std::string &weightDef);

  bool isUnderline() const;
  void setUnderline(bool b=true);

  bool isOverline() const;
  void setOverline(bool b=true);

  bool isLineThrough() const;
  void setLineThrough(bool b=true);

  bool isSubscript() const;
  void setSubscript(bool b=true);

  bool isSuperscript() const;
  void setSuperscript(bool b=true);

  void textSize(const std::string &text, double *w, double *a, double *d) const;

  CSVGFontObj *getObj() const;

  void print(std::ostream &os) const;

 private:
  void resetObj();

 private:
  CSVG&                 svg_;
  COptValT<Family>      family_;
  COptValT<FontSize>    size_;
  COptValT<CFontStyles> style_;
  COptReal              angle_;
  mutable CSVGFontObj*  obj_ { nullptr };
};

#endif
