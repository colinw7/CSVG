#include <CSVGFontFaceSrc.h>
#include <CSVG.h>

CSVGFontFaceSrc::
CSVGFontFaceSrc(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGFontFaceSrc::
CSVGFontFaceSrc(const CSVGFontFaceSrc &face) :
 CSVGObject(face)
{
}

CSVGFontFaceSrc *
CSVGFontFaceSrc::
dup() const
{
  return new CSVGFontFaceSrc(*this);
}

bool
CSVGFontFaceSrc::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

bool
CSVGFontFaceSrc::
draw()
{
  return false;
}

void
CSVGFontFaceSrc::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<font-face-src";

    printValues(os);

    os << ">\n";

    if (hasChildren()) {
      printChildren(os, hier);

      os << "</font-face-src>\n";
    }
    else
      os << "/>\n";
  }
  else
    os << "font-face-src";
}

void
CSVGFontFaceSrc::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);
}
