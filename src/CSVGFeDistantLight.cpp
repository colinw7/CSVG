#include <CSVGFeDistantLight.h>
#include <CSVG.h>

CSVGFeDistantLight::
CSVGFeDistantLight(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeDistantLight::
CSVGFeDistantLight(const CSVGFeDistantLight &fe) :
 CSVGFilterBase(fe)
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
  double r;

  if      (svg_.realOption(opt_name, opt_value, "elevation", &r))
    elevation_ = r;
  else if (svg_.realOption(opt_name, opt_value, "azimuth", &r))
    azimuth_ = r;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGFeDistantLight::
draw()
{
  return true;
}

void
CSVGFeDistantLight::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feDistantLight";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "elevation", elevation_);
    printNameValue(os, "azimuth"  , azimuth_  );

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
