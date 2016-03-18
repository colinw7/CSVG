#include <CSVGI.h>

CSVGHKern::
CSVGHKern(CSVG &svg) :
 CSVGObject(svg),
 glyph1_   (),
 glyph2_   (),
 k_        ()
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

void
CSVGHKern::
draw()
{
}

void
CSVGHKern::
print(std::ostream &os) const
{
  os << "glyph";
}

std::ostream &
operator<<(std::ostream &os, const CSVGHKern &glyph)
{
  glyph.print(os);

  return os;
}
