#include <CSVGI.h>
#include <CFontMgr.h>

CSVGFontDef::
CSVGFontDef(CSVG &svg) :
 svg_   (svg),
 family_(),
 size_  (),
 style_ ()
{
}

CSVGFontDef::
CSVGFontDef(const CSVGFontDef &font_def) :
 svg_   (font_def.svg_),
 family_(font_def.family_),
 size_  (font_def.size_),
 style_ (font_def.style_)
{
}

const CSVGFontDef &
CSVGFontDef::
operator=(const CSVGFontDef &font_def)
{
  family_ = font_def.family_;
  size_   = font_def.size_  ;
  style_  = font_def.style_;

  return *this;
}

void
CSVGFontDef::
reset()
{
  family_.setInvalid();
  size_  .setInvalid();
  style_ .setInvalid();
}

string
CSVGFontDef::
getFamily() const
{
  if (hasFamily())
    return family_.getValue();
  else
    return "serif";
}

void
CSVGFontDef::
setFamily(const string &family)
{
  family_ = family;
}

double
CSVGFontDef::
getSize() const
{
  if (hasSize())
    return size_.getValue();
  else
    return 10;
}

void
CSVGFontDef::
setSize(double size)
{
  size_ = size;
}

void
CSVGFontDef::
setWeight(const string &weight_def)
{
  CFontStyles weight = svg_.decodeFontWeightString(weight_def);

  if (hasStyle())
    style_.setValue(style_.getValue() | weight);
  else
    style_.setValue(weight);
}

CFontStyles
CSVGFontDef::
getStyle() const
{
  if (hasStyle())
    return style_.getValue();
  else
    return CFONT_STYLE_NORMAL;
}

void
CSVGFontDef::
setStyle(const string &style_def)
{
  CFontStyles style = svg_.decodeFontStyleString(style_def);

  if (hasStyle())
    style_.setValue(style_.getValue() | style);
  else
    style_.setValue(style);
}
