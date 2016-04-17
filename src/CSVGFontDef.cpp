#include <CSVGFontDef.h>
#include <CSVG.h>
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

std::string
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
setFamily(const std::string &family)
{
  family_ = family;
}

double
CSVGFontDef::
getSize() const
{
  if (hasSize())
    return size_.getValue().value();
  else
    return 10;
}

void
CSVGFontDef::
setSize(const CSVGLengthValue &size)
{
  size_ = size;
}

void
CSVGFontDef::
setSize(double size)
{
  size_ = CSVGLengthValue(CSVGLengthType::NONE, size);
}

void
CSVGFontDef::
setWeight(const std::string &weight_def)
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
setStyle(const std::string &style_def)
{
  CFontStyles style = svg_.decodeFontStyleString(style_def);

  if (hasStyle())
    style_.setValue(style_.getValue() | style);
  else
    style_.setValue(style);
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
