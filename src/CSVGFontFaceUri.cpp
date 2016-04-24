#include <CSVGFontFaceUri.h>
#include <CSVG.h>

CSVGFontFaceUri::
CSVGFontFaceUri(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGFontFaceUri::
CSVGFontFaceUri(const CSVGFontFaceUri &face) :
 CSVGObject(face),
 xlink_    (face.xlink_)
{
}

CSVGFontFaceUri *
CSVGFontFaceUri::
dup() const
{
  return new CSVGFontFaceUri(*this);
}

void
CSVGFontFaceUri::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

bool
CSVGFontFaceUri::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFontFaceUri::
draw()
{
}

void
CSVGFontFaceUri::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<font-face-uri";

    printNameXLink(os, "xlink:href", xlink_);

    CSVGObject::printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "font-face-uri";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFontFaceUri &face)
{
  face.print(os, false);

  return os;
}
