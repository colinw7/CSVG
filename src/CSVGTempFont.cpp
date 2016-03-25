#include <CSVGTempFont.h>
#include <CSVGObject.h>
#include <CSVG.h>

CSVGTempFont::
CSVGTempFont(CSVGObject &object) :
 object_  (object),
 font_def_(object_.getFontDef())
{
  font_def_ = object_.getSVG().getFontDef();

  object_.getSVG().updateFontDef(object_.getFontDef());
}

CSVGTempFont::
~CSVGTempFont()
{
  object_.getSVG().updateFontDef(font_def_);
}
