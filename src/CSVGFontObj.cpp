#include <CSVGFontObj.h>
#include <CSVGFontDef.h>
#include <CFontMgr.h>

CSVGFontObj::
CSVGFontObj(const CSVGFontDef &fontDef) :
 fontDef_(fontDef)
{
  std::string family = fontDef.getFamily().getValue();
  CFontStyles styles = fontDef.getStyle();
  CFontStyle  style  = (styles | CFONT_STYLE_FULL_SIZE).value();
  double      size   = fontDef.getSize().getValue().px().value();
  double      angle  = fontDef.getAngle();

  font_ = CFontMgrInst->lookupFont(family, style, size, angle);
}

void
CSVGFontObj::
textSize(const std::string &text, double *w, double *a, double *d) const
{
  if (w) *w = font_->getStringWidth(text);
  if (a) *a = font_->getCharAscent();
  if (d) *d = font_->getCharDescent();
}
