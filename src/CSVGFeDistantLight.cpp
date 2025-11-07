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
drawElement()
{
  return true;
}

void
CSVGFeDistantLight::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feDistantLight";

    printValues(os);

    os << "/>\n";
  }
  else
    os << "feDistantLight ";
}

void
CSVGFeDistantLight::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  CSVGFilterBase::printValues(os, flat);

  printNameValue(os, "elevation", elevation_);
  printNameValue(os, "azimuth"  , azimuth_  );
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeDistantLight &fe)
{
  fe.print(os, false);

  return os;
}
