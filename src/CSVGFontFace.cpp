#include <CSVGFontFace.h>
#include <CSVG.h>

CSVGFontFace::
CSVGFontFace(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGFontFace::
CSVGFontFace(const CSVGFontFace &face) :
 CSVGObject(face),
 fontFamily_(face.fontFamily_),
 fontStyle_ (face.fontStyle_),
 unitsPerEm_(face.unitsPerEm_),
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
    fontFamily_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "font-style", str))
    fontStyle_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "font-variant", str))
    fontVariant_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "font-weight", str))
    fontWeight_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "font-stretch", str))
    fontStretch_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "font-size", str))
    fontSize_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "unicode-range", str))
    unicodeRange_ = str;
  else if (svg_.integerOption(opt_name, opt_value, "units-per-em", &integer))
    unitsPerEm_ = integer;
  else if (svg_.stringOption (opt_name, opt_value, "panose-1", str))
    panose_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "stemv", str))
    stemv_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "stemh", str))
    stemh_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "slope", str))
    slope_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "cap-height", str))
    capHeight_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "x-height", str))
    xheight_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "accent-height", str))
    accentHeight_ = str;
  else if (svg_.integerOption(opt_name, opt_value, "ascent", &integer))
    ascent_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "descent", &integer))
    descent_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "baseline", &integer))
    baseline_ = integer;
  else if (svg_.stringOption (opt_name, opt_value, "widths", str))
    widths_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "bbox", str))
    bbox_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "ideographic", str))
    ideographic_ = str;
  else if (svg_.integerOption(opt_name, opt_value, "alphabetic", &integer))
    alphabetic_ = (integer != 0);
  else if (svg_.stringOption (opt_name, opt_value, "mathematical", str))
    mathematical_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "hanging", str))
    hanging_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "v-ideographic", str))
    vIdeographic_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "v-alphabetic", str))
    vAlphabetic_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "v-mathematical", str))
    vMathematical_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "v-hanging", str))
    vHanging_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "underline-position", str))
    underlinePosition_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "underline-thickness", str))
    underlineThickness_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "strikethrough-position", str))
    strikethroughPosition_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "strikethrough-thickness", str))
    strikethroughThickness_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "overline-position", str))
    overlinePosition_ = str;
  else if (svg_.stringOption (opt_name, opt_value, "overline-thickness", str))
    overlineThickness_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFontFace::
draw()
{
  return false;
}

void
CSVGFontFace::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<font-face";

    printValues(os);

    os << ">" << std::endl;

    if (hasChildren()) {
      printChildren(os, hier);

      os << "</font-face>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "font-face";
}

void
CSVGFontFace::
printValues(std::ostream &os, bool flat) const
{
  printNameValue(os, "font-family" , fontFamily_);
  printNameValue(os, "font-style"  , fontStyle_);
  printNameValue(os, "units-per-em", unitsPerEm_);
  printNameValue(os, "panose-1"    , panose_);
  printNameValue(os, "ascent"      , ascent_);
  printNameValue(os, "descent"     , descent_);
  printNameValue(os, "alphabetic"  , alphabetic_);

  CSVGObject::printValues(os, flat);
}

std::ostream &
operator<<(std::ostream &os, const CSVGFontFace &face)
{
  face.print(os, false);

  return os;
}
