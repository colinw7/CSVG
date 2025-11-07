#ifndef CSVGFontDef_H
#define CSVGFontDef_H

#include <CSVGInheritVal.h>
#include <CScreenUnits.h>
#include <CFontStyle.h>
#include <optional>

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

  bool isSet() const { return (family_ || size_ || style_); }

  const CSVG &svg() const { return svg_; }

  void reset();

  bool hasFamily() const { return !!family_; }
  Family getFamily() const { return family_.value_or(Family("serif")); }
  void setFamily(const Family &family);

  bool hasSize() const { return !!size_; }
  FontSize getSize() const { return size_.value_or(FontSize(CScreenUnits(12))); }
  void setSize(const FontSize &lvalue);

  bool hasStyle() const { return !!style_; }
  CFontStyles getStyle() const { return style_.value_or(CFontStyles(CFONT_STYLE_NORMAL)); }
  void setStyle(CFontStyles s);

  bool hasAngle() const { return !!angle_; }
  double getAngle() const { return angle_.value_or(0); }
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
  CSVG&                      svg_;
  std::optional<Family>      family_;
  std::optional<FontSize>    size_;
  std::optional<CFontStyles> style_;
  std::optional<double>      angle_;
  mutable CSVGFontObj*       obj_ { nullptr };
};

#endif
