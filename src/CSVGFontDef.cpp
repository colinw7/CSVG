#include <CSVGFontDef.h>
#include <CSVGFontObj.h>
#include <CSVG.h>

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
 style_ (font_def.style_),
 angle_ (font_def.angle_)
{
}

CSVGFontDef &
CSVGFontDef::
operator=(const CSVGFontDef &font_def)
{
  family_ = font_def.family_;
  size_   = font_def.size_  ;
  style_  = font_def.style_;
  angle_  = font_def.angle_;

  resetObj();

  return *this;
}

void
CSVGFontDef::
reset()
{
  family_.setInvalid();
  size_  .setInvalid();
  style_ .setInvalid();
  angle_ .setInvalid();

  resetObj();
}

void
CSVGFontDef::
setFamily(const std::string &family)
{
  family_ = family;

  resetObj();
}

void
CSVGFontDef::
setSize(const CScreenUnits &size)
{
  size_ = size;

  resetObj();
}

void
CSVGFontDef::
setWeight(const std::string &weight_def)
{
  CFontStyles weight = CFontStyles(svg_.decodeFontWeightString(weight_def));

  if (hasStyle())
    style_.setValue(getStyle() | weight);
  else
    style_.setValue(weight);

  resetObj();
}

void
CSVGFontDef::
setStyle(const std::string &style_def)
{
  CFontStyles style = CFontStyles(svg_.decodeFontStyleString(style_def));

  if (hasStyle())
    style_.setValue(getStyle() | style);
  else
    style_.setValue(style);

  resetObj();
}

void
CSVGFontDef::
setStyle(CFontStyles s)
{
  style_.setValue(s);

  resetObj();
}

bool
CSVGFontDef::
isUnderline() const
{
  return (hasStyle() && (getStyle() & CFONT_STYLE_UNDERLINE));
}

void
CSVGFontDef::
setUnderline(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_UNDERLINE, b));
}

bool
CSVGFontDef::
isOverline() const
{
  return (hasStyle() && (getStyle() & CFONT_STYLE_OVERLINE));
}

void
CSVGFontDef::
setOverline(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_OVERLINE, b));
}

bool
CSVGFontDef::
isLineThrough() const
{
  return (hasStyle() && (getStyle() & CFONT_STYLE_STRIKEOUT));
}

void
CSVGFontDef::
setLineThrough(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_STRIKEOUT, b));
}

bool
CSVGFontDef::
isSubscript() const
{
  return (hasStyle() && (getStyle() & CFONT_STYLE_SUBSCRIPT));
}

void
CSVGFontDef::
setSubscript(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_SUBSCRIPT, b));
}

bool
CSVGFontDef::
isSuperscript() const
{
  return (hasStyle() && (getStyle() & CFONT_STYLE_SUPERSCRIPT));
}

void
CSVGFontDef::
setSuperscript(bool b)
{
  style_.setValue(getStyle().addRemove(CFONT_STYLE_SUPERSCRIPT, b));
}

void
CSVGFontDef::
setAngle(double a)
{
  angle_ = a;
}

CSVGFontObj *
CSVGFontDef::
getObj() const
{
  if (! obj_)
    obj_ = svg_.createFontObj(*this);

  return obj_;
}

void
CSVGFontDef::
resetObj()
{
  delete obj_;

  obj_ = nullptr;
}

void
CSVGFontDef::
textSize(const std::string &text, double *w, double *a, double *d) const
{
  CSVGFontObj *obj = getObj();

  obj->textSize(text, w, a, d);
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

    os << "font-size: " << size_.getValue() << ";";

    output = true;
  }

  if (style_.isValid()) {
    CFontStyles styles = style_.getValue();

    if (styles & CFONT_STYLE_BOLD) {
      if (output) os << " ";

      os << "font-weight: bold;";

      output = true;
    }

    if (styles & CFONT_STYLE_ITALIC) {
      if (output) os << " ";

      os << "font-style: italic;";

      output = true;
    }

    if (isUnderline() || isOverline() || isLineThrough()) {
      if (output) os << " ";

      if      (isUnderline())
        os << "text-decoration:underline;";
      else if (isOverline())
        os << "text-decoration:overline;";
      else if (isLineThrough())
        os << "text-decoration:line-through;";

      output = true;
    }
  }

  // TODO: angle
}
