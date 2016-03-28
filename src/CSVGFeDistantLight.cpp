#include <CSVGFeDistantLight.h>
#include <CSVG.h>

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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feDistantLight";

    CSVGObject::printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "feDistantLight ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeDistantLight &fe)
{
  fe.print(os, false);

  return os;
}
