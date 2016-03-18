#include <CSVGI.h>

CSVGMissingGlyph::
CSVGMissingGlyph(CSVG &svg) :
 CSVGGlyph(svg)
{
}

bool
CSVGMissingGlyph::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return CSVGGlyph::processOption(opt_name, opt_value);
}

void
CSVGMissingGlyph::
draw()
{
}

void
CSVGMissingGlyph::
print(std::ostream &os) const
{
  os << "missing-glyph";
}

std::ostream &
operator<<(std::ostream &os, const CSVGMissingGlyph &glyph)
{
  glyph.print(os);

  return os;
}
