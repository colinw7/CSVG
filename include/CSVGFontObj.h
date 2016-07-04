#ifndef CSVGFontObj_H
#define CSVGFontObj_H

#include <CFont.h>

class CSVGFontDef;

class CSVGFontObj {
 public:
  CSVGFontObj(const CSVGFontDef &def);

  virtual ~CSVGFontObj() { }

  CFontPtr font() { return font_; }

  void textSize(const std::string &text, double *w, double *a, double *d) const;

 private:
  CFontPtr font_;
};

#endif
