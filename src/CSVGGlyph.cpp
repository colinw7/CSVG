#include <CSVGGlyph.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGGlyph::
CSVGGlyph(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGGlyph::
CSVGGlyph(const CSVGGlyph &glyph) :
 CSVGObject(glyph),
 unicode_  (glyph.unicode_),
 name_     (glyph.name_),
 dx_       (glyph.dx_),
 parts_    (glyph.parts_)
{
}

CSVGGlyph *
CSVGGlyph::
dup() const
{
  return new CSVGGlyph(*this);
}

bool
CSVGGlyph::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  long        integer;

  if      (svg_.stringOption(opt_name, opt_value, "unicode", str)) {
    unicode_ = str;
  }
  else if (svg_.stringOption(opt_name, opt_value, "glyph-name", str)) {
    name_ = str;
  }
  else if (svg_.stringOption(opt_name, opt_value, "d", str)) {
    if (! svg_.pathStringToParts(str, parts_))
      return false;
  }
  else if (svg_.stringOption(opt_name, opt_value, "orientation", str)) {
    ;
  }
  else if (svg_.stringOption(opt_name, opt_value, "arabic-form", str)) {
    ;
  }
  else if (svg_.stringOption(opt_name, opt_value, "lang", str)) {
    ;
  }
  else if (svg_.integerOption(opt_name, opt_value, "horiz-adv-x", &integer)) {
    dx_ = integer;
  }
  else if (svg_.integerOption(opt_name, opt_value, "vert-origin-x", &integer)) {
    ;
  }
  else if (svg_.integerOption(opt_name, opt_value, "vert-origin-y", &integer)) {
    ;
  }
  else if (svg_.integerOption(opt_name, opt_value, "vert-adv-y", &integer)) {
    ;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGGlyph::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.drawParts(parts_);
}

void
CSVGGlyph::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<glyph";

    printNameValue(os, "unicode"    , unicode_);
    printNameValue(os, "glyph-name" , name_   );
    printNameValue(os, "horiz-adv-x", dx_     );

    os << " d=\"";

    svg_.printParts(os, parts_);

    os << "\"";

    os << "/>" << std::endl;
  }
  else {
    os << "glyph " << unicode_ << " " << name_ << "(";

    svg_.printParts(os, parts_);

    os << ")";
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGGlyph &glyph)
{
  glyph.print(os, false);

  return os;
}
