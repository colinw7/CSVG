#include <CSVGHKern.h>
#include <CSVG.h>

CSVGHKern::
CSVGHKern(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGHKern::
CSVGHKern(const CSVGHKern &hk) :
 CSVGObject(hk),
 glyph1_   (hk.glyph1_),
 glyph2_   (hk.glyph2_),
 k_        (hk.k_)
{
}

CSVGHKern *
CSVGHKern::
dup() const
{
  return new CSVGHKern(*this);
}

bool
CSVGHKern::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  long        integer;

  if      (svg_.stringOption(opt_name, opt_value, "u1", str)) {
    ;
  }
  else if (svg_.stringOption(opt_name, opt_value, "g1", str)) {
    glyph1_ = str;
  }
  else if (svg_.stringOption(opt_name, opt_value, "u2", str)) {
    ;
  }
  else if (svg_.stringOption(opt_name, opt_value, "g2", str)) {
    glyph2_ = str;
  }
  else if (svg_.integerOption(opt_name, opt_value, "k", &integer)) {
    k_ = integer;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGHKern::
draw()
{
  return false;
}

void
CSVGHKern::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<hkern";

    printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "hkern";
}

void
CSVGHKern::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue(os, "g1", glyph1_);
  printNameValue(os, "g2", glyph2_);
  printNameValue(os, "k" , k_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGHKern &glyph)
{
  glyph.print(os, false);

  return os;
}
