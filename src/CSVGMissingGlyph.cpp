#include <CSVGI.h>

CSVGMissingGlyph::
CSVGMissingGlyph(CSVG &svg) :
 CSVGGlyph(svg)
{
}

bool
CSVGMissingGlyph::
processOption(const string &opt_name, const string &opt_value)
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
print(ostream &os) const
{
  os << "missing-glyph";
}

ostream &
operator<<(ostream &os, const CSVGMissingGlyph &glyph)
{
  glyph.print(os);

  return os;
}
