#ifndef CSVGFontDef_H
#define CSVGFontDef_H

#include <CSVGLengthValue.h>
#include <CFont.h>
#include <COptVal.h>

class CSVG;

class CSVGFontDef {
 public:
  CSVGFontDef(CSVG &svg);
  CSVGFontDef(const CSVGFontDef &font_def);

  const CSVGFontDef &operator=(const CSVGFontDef &font_def);

  bool isSet() const {
    return (family_.isValid() || size_.isValid() || style_.isValid());
  }

  void reset();

  bool hasFamily() const { return family_.isValid(); }
  std::string getFamily() const { return family_.getValue("serif"); }
  void setFamily(const std::string &family);

  bool hasSize() const { return size_  .isValid(); }
  double getSize() const { return size_.getValue(CSVGLengthValue(8)).value(); }
  void setSize(double size);
  void setSize(const CSVGLengthValue &lvalue);

  bool hasStyle () const { return style_ .isValid(); }
  CFontStyles getStyle () const { return style_.getValue(CFONT_STYLE_NORMAL); }
  void setStyle (const std::string &style_def);
  void setStyle (CFontStyle s);

  void setWeight(const std::string &weight_def);

  void setUnderline(bool b=true);
  void setOverline(bool b=true);
  void setLineThrough(bool b=true);

  void setSubscript(bool b=true);
  void setSuperscript(bool b=true);

  void print(std::ostream &os) const;

 private:
  CSVG&                     svg_;
  COptValT<std::string>     family_;
  COptValT<CSVGLengthValue> size_;
  COptValT<CFontStyles>     style_;
};

#endif
