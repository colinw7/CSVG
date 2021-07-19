#include <CSVGFontDef.h>
#include <CSVGFontObj.h>
#include <CSVG.h>

CSVGFontDef::
CSVGFontDef(CSVG &svg) :
 svg_(svg)
{
}

CSVGFontDef::
CSVGFontDef(const CSVGFontDef &fontDef) :
 svg_   (fontDef.svg_),
 family_(fontDef.family_),
 size_  (fontDef.size_),
 style_ (fontDef.style_),
 angle_ (fontDef.angle_)
{
}

CSVGFontDef &
CSVGFontDef::
operator=(const CSVGFontDef &fontDef)
{
  family_ = fontDef.family_;
  size_   = fontDef.size_  ;
  style_  = fontDef.style_;
  angle_  = fontDef.angle_;

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
setFamily(const Family &family)
{
  family_ = family;

  resetObj();
}

void
CSVGFontDef::
setSize(const FontSize &size)
{
  size_ = size;

  resetObj();
}

void
CSVGFontDef::
setWeight(const std::string &weightDef)
{
  bool inherit;

  auto weight = CFontStyles(svg_.decodeFontWeightString(weightDef, inherit));

  if (hasStyle())
    style_.setValue(getStyle() | weight);
  else
    style_.setValue(weight);

  resetObj();
}

void
CSVGFontDef::
setStyle(const std::string &styleDef)
{
  bool inherit;

  auto style = CFontStyles(svg_.decodeFontStyleString(styleDef, inherit));

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
  auto *obj = getObj();

  obj->textSize(text, w, a, d);
}

void
CSVGFontDef::
print(std::ostream &os) const
{
  bool output = false;

  if (family_.isValid()) {
    os << "font-family: ";

    if (family_.getValue().isInherit())
      os << "inherit";
    else
      os << family_.getValue().getValue();

    os << ";";

    output = true;
  }

  if (size_.isValid()) {
    if (output) os << " ";

    os << "font-size: ";

    if (size_.getValue().isInherit())
      os << "inherit";
    else
      os << size_.getValue().getValue();

    os << ";";

    output = true;
  }

  if (style_.isValid()) {
    auto styles = style_.getValue();

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
