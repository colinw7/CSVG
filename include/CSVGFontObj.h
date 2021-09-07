#ifndef CSVGFontObj_H
#define CSVGFontObj_H

#include <CSVGFontDef.h>
#include <CFont.h>

class CSVGFontDef;

class CSVGFontObj {
 public:
  CSVGFontObj(const CSVGFontDef &def);

  virtual ~CSVGFontObj() { }

  const CSVGFontDef &fontDef() const { return fontDef_; }

  CFontPtr font() { return font_; }

  void textSize(const std::string &text, double *w, double *a, double *d) const;

 private:
  CSVGFontDef fontDef_;
  CFontPtr    font_;
};

#endif
