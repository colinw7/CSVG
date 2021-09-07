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
 CSVGObject  (glyph),
 unicode_    (glyph.unicode_),
 glyphName_  (glyph.glyphName_),
 parts_      (glyph.parts_),
 orientation_(glyph.orientation_),
 arabicForm_ (glyph.arabicForm_),
 lang_       (glyph.lang_),
 horizAdvX_  (glyph.horizAdvX_),
 vertAdvY_   (glyph.vertAdvY_),
 vertOriginX_(glyph.vertOriginX_),
 vertOriginY_(glyph.vertOriginY_)
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
  std::string      str;
  CSVGPathPartList parts;
  long             integer;

  if      (svg_.stringOption(opt_name, opt_value, "unicode", str))
    unicode_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "glyph-name", str))
    glyphName_ = str;
  else if (svg_.pathOption  (opt_name, opt_value, "d", parts))
    parts_ = parts;
  else if (svg_.stringOption(opt_name, opt_value, "orientation", str))
    orientation_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "arabic-form", str))
    arabicForm_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "lang", str))
    lang_ = str;
  else if (svg_.integerOption(opt_name, opt_value, "horiz-adv-x", &integer))
    horizAdvX_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "vert-adv-y", &integer))
    vertAdvY_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "vert-origin-x", &integer))
    vertOriginX_ = integer;
  else if (svg_.integerOption(opt_name, opt_value, "vert-origin-y", &integer))
    vertOriginY_ = integer;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGGlyph::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.drawParts(parts_);

  return true;
}

void
CSVGGlyph::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<glyph";

    printValues(os);

    os << "/>" << std::endl;
  }
  else {
    os << "glyph " << unicode_ << " " << glyphName_ << "(" << parts_ << ")";
  }
}

void
CSVGGlyph::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue(os, "unicode"      , unicode_    );
  printNameValue(os, "glyph-name"   , glyphName_  );
  printNameParts(os, "d"            , parts_      );
  printNameValue(os, "orientation"  , orientation_);
  printNameValue(os, "arabic-form"  , arabicForm_ );
  printNameValue(os, "lang"         , lang_       );
  printNameValue(os, "horiz-adv-x"  , horizAdvX_  );
  printNameValue(os, "vert-adv-y"   , vertAdvY_   );
  printNameValue(os, "vert-origin-x", vertOriginX_);
  printNameValue(os, "vert-origin-y", vertOriginY_);
}
