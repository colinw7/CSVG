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

void
CSVGMissingGlyph::
draw()
{
}

void
CSVGMissingGlyph::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<missing-glyph";

    printNameValue(os, "horiz-adv-x", hax_);

    os << " d=\"";

    svg_.printParts(os, parts_);

    os << "\"";

    os << "/>" << std::endl;
  }
  else {
    os << "missing-glyph " << "(";

    svg_.printParts(os, parts_);

    os << ")";
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGMissingGlyph &glyph)
{
  glyph.print(os, false);

  return os;
}
