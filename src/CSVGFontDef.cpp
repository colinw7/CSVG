#include <CSVGFontDef.h>
#include <CSVG.h>
#include <CFontMgr.h>

CSVGFontDef::
CSVGFontDef(CSVG &svg) :
 svg_(svg)
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

void
CSVGFontDef::
setFamily(const std::string &family)
{
  family_ = family;
}

void
CSVGFontDef::
setSize(const CSVGLengthValue &size)
{
  size_ = size;
}

void
CSVGFontDef::
setWeight(const std::string &weight_def)
{
  CFontStyles weight = svg_.decodeFontWeightString(weight_def);

  if (hasStyle())
    style_.setValue(getStyle() | weight);
  else
    style_.setValue(weight);
}

void
CSVGFontDef::
setStyle(const std::string &style_def)
{
  CFontStyles style = svg_.decodeFontStyleString(style_def);

  if (hasStyle())
    style_.setValue(getStyle() | style);
  else
    style_.setValue(style);
}

void
CSVGFontDef::
setStyle(CFontStyle s)
{
  style_.setValue(s);
}

void
CSVGFontDef::
setUnderline(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_UNDERLINE, b));
}

void
CSVGFontDef::
setOverline(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_OVERLINE, b));
}

void
CSVGFontDef::
setLineThrough(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_STRIKEOUT, b));
}

void
CSVGFontDef::
setSubscript(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_SUBSCRIPT, b));
}

void
CSVGFontDef::
setSuperscript(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_SUPERSCRIPT, b));
}

void
CSVGFontDef::
print(std::ostream &os) const
{
  bool output = false;

  if (family_.isValid()) {
    os << "font-family: " << family_.getValue() << ";";

    output = true;
  }

  if (size_.isValid()) {
    if (output) os << " ";

    os << "font-size: "; CSVGObject::printLength(os, size_.getValue()); os << ";";

    output = true;
  }

  if (style_.isValid()) {
    // TODO:

    output = true;
  }
}
