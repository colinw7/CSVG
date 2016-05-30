#ifndef CSVGTempFont_H
#define CSVGTempFont_H

#include <CSVGFontDef.h>

class CSVGObject;

class CSVGTempFont {
 public:
  CSVGTempFont(CSVGObject &object);
 ~CSVGTempFont();

 private:
  CSVGObject  &object_;
  CSVGFontDef  fontDef_;
};

#endif
