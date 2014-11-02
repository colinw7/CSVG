#include <CSVGI.h>

CSVGFeDistantLight::
CSVGFeDistantLight(CSVG &svg) :
 CSVGFilter(svg)
{
}

CSVGFeDistantLight::
CSVGFeDistantLight(const CSVGFeDistantLight &fe) :
 CSVGFilter(fe)
{
}

CSVGFeDistantLight *
CSVGFeDistantLight::
dup() const
{
  return new CSVGFeDistantLight(*this);
}

bool
CSVGFeDistantLight::
processOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "elevation", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "azimuth", str))
    ;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeDistantLight::
draw()
{
}

void
CSVGFeDistantLight::
print(ostream &os) const
{
  os << "feDistantLight ";
}

ostream &
operator<<(ostream &os, const CSVGFeDistantLight &fe)
{
  fe.print(os);

  return os;
}
