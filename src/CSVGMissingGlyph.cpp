#include <CSVGMissingGlyph.h>
#include <CSVG.h>

CSVGMissingGlyph::
CSVGMissingGlyph(CSVG &svg) :
 CSVGGlyph(svg)
{
}

bool
CSVGMissingGlyph::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  long integer;

  if (svg_.integerOption(opt_name, opt_value, "horiz-adv-x", &integer))
    hax_ = integer;
  else
    return CSVGGlyph::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGMissingGlyph::
draw()
{
  return false;
}

void
CSVGMissingGlyph::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<missing-glyph";

    printNameValue(os, "horiz-adv-x", hax_);

    os << " d=\"" << parts_ << "\"";

    os << "/>" << std::endl;
  }
  else {
    os << "missing-glyph " << "(" << parts_ << ")";
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGMissingGlyph &glyph)
{
  glyph.print(os, false);

  return os;
}
