#include <CSVGI.h>

CSVGFePointLight::
CSVGFePointLight(CSVG &svg) :
 CSVGFilter(svg)
{
}

CSVGFePointLight::
CSVGFePointLight(const CSVGFePointLight &fe) :
 CSVGFilter (fe)
{
}

CSVGFePointLight *
CSVGFePointLight::
dup() const
{
  return new CSVGFePointLight(*this);
}

bool
CSVGFePointLight::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "x", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "y", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "z", str))
    ;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFePointLight::
draw()
{
}

void
CSVGFePointLight::
print(std::ostream &os) const
{
  os << "fePointLight ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFePointLight &fe)
{
  fe.print(os);

  return os;
}
