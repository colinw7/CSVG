#include <CSVGI.h>

CSVGFontFace::
CSVGFontFace(CSVG &svg) :
 CSVGObject(svg),
 family_    ("courier"),
 units_     (0),
 panose_    (""),
 ascent_    (0),
 descent_   (0),
 alphabetic_(false)
{
}

CSVGFontFace::
CSVGFontFace(const CSVGFontFace &face) :
 CSVGObject(face),
 family_    (face.family_),
 units_     (face.units_),
 panose_    (face.panose_),
 ascent_    (face.ascent_),
 descent_   (face.descent_),
 alphabetic_(face.alphabetic_)
{
}

CSVGFontFace *
CSVGFontFace::
dup() const
{
  return new CSVGFontFace(*this);
}

bool
CSVGFontFace::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  long        integer;

  if      (svg_.stringOption (opt_name, opt_value, "font-family", str))
    family_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "font-style", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "font-variant", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "font-weight", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "font-stretch", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "font-size", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "unicode-range", str))
    ;
  else if (svg_.integerOption(opt_name, opt_value, "units-per-em", &integer))
    units_ = integer;
  else if (svg_.stringOption (opt_name, opt_value, "panose-1", str))
    panose_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "stemv", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "stemh", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "slope", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "cap-height", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "x-height", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "accent-height", str))
    ;
  else if (svg_.integerOption(opt_name, opt_value, "ascent", &integer))
    ascent_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "descent", &integer))
    descent_ = integer;
  else if (svg_.stringOption (opt_name, opt_value, "widths", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "bbox", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "ideographic", str))
    ;
  else if (svg_.integerOption(opt_name, opt_value, "alphabetic", &integer))
    alphabetic_ = (integer != 0);
  else if (svg_.stringOption (opt_name, opt_value, "mathematical", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "hanging", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "v-ideographic", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "v-alphabetic", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "v-mathematical", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "v-hanging", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "underline-position", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "underline-thickness", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "strikethrough-position", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "strikethrough-thickness", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "overline-position", str))
    ;
  else if (svg_.stringOption (opt_name, opt_value, "overline-thickness", str))
    ;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFontFace::
draw()
{
}

void
CSVGFontFace::
print(std::ostream &os) const
{
  os << "font-face";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFontFace &face)
{
  face.print(os);

  return os;
}
