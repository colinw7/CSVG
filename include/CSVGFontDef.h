#ifndef CSVGFontDef_H
#define CSVGFontDef_H

#include <CScreenUnits.h>
#include <CFontStyle.h>
#include <COptVal.h>

class CSVG;
class CSVGFontObj;

class CSVGFontDef {
 public:
  CSVGFontDef(CSVG &svg);
  CSVGFontDef(const CSVGFontDef &font_def);

  CSVGFontDef &operator=(const CSVGFontDef &font_def);

  bool isSet() const {
    return (family_.isValid() || size_.isValid() || style_.isValid());
  }

  void reset();

  bool hasFamily() const { return family_.isValid(); }
  std::string getFamily() const { return family_.getValue("serif"); }
  void setFamily(const std::string &family);

  bool hasSize() const { return size_.isValid(); }
  CScreenUnits getSize() const { return size_.getValue(CScreenUnits(12)); }
  void setSize(const CScreenUnits &lvalue);

  bool hasStyle() const { return style_.isValid(); }
  CFontStyles getStyle() const { return style_.getValue(CFontStyles(CFONT_STYLE_NORMAL)); }
  void setStyle(CFontStyles s);

  bool hasAngle() const { return angle_.isValid(); }
  double getAngle() const { return angle_.getValue(0); }
  void setAngle(double a);

  void setStyle (const std::string &style_def );
  void setWeight(const std::string &weight_def);

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
  CSVG&                  svg_;
  COptString             family_;
  COptValT<CScreenUnits> size_;
  COptValT<CFontStyles>  style_;
  COptReal               angle_;
  mutable CSVGFontObj*   obj_ { 0 };
};

#endif
