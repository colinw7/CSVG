#include <CSVGTempFont.h>
#include <CSVGObject.h>
#include <CSVG.h>

CSVGTempFont::
CSVGTempFont(CSVGObject &object) :
 object_ (object),
 fontDef_(object_.getFontDef())
{
  fontDef_ = object_.getSVG().getFontDef();

  object_.getSVG().updateFontDef(object_.getFontDef());
}

CSVGTempFont::
~CSVGTempFont()
{
  object_.getSVG().updateFontDef(fontDef_);
}
