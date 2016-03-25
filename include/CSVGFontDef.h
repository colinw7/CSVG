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

  void setFamily(const std::string &family);
  void setSize  (double size);
  void setSize  (const CSVGLengthValue &lvalue);
  void setWeight(const std::string &weight_def);
  void setStyle (const std::string &style_def);

  bool hasFamily() const { return family_.isValid(); }
  bool hasSize  () const { return size_  .isValid(); }
  bool hasStyle () const { return style_ .isValid(); }

  std::string getFamily() const;
  double      getSize  () const;
  CFontStyles getStyle () const;

  void print(std::ostream &os) const;

 private:
  CSVG&                     svg_;
  COptValT<std::string>     family_;
  COptValT<CSVGLengthValue> size_;
  COptValT<CFontStyles>     style_;
};

#endif
