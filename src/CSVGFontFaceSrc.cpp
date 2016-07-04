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

    os << ">" << std::endl;

    if (hasChildren()) {
      printChildren(os, hier);

      os << "</font-face-src>" << std::endl;
    }
    else
      os << "/>" << std::endl;
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

std::ostream &
operator<<(std::ostream &os, const CSVGFontFaceSrc &face)
{
  face.print(os, false);

  return os;
}
