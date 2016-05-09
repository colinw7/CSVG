#include <CSVGFeSpotLight.h>
#include <CSVG.h>

CSVGFeSpotLight::
CSVGFeSpotLight(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeSpotLight::
CSVGFeSpotLight(const CSVGFeSpotLight &fe) :
 CSVGFilterBase(fe)
{
}

CSVGFeSpotLight *
CSVGFeSpotLight::
dup() const
{
  return new CSVGFeSpotLight(*this);
}

bool
CSVGFeSpotLight::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  double r;

  if      (svg_.realOption(opt_name, opt_value, "x", &r))
    x_ = r;
  else if (svg_.realOption(opt_name, opt_value, "y", &r))
    y_ = r;
  else if (svg_.realOption(opt_name, opt_value, "z", &r))
    z_ = r;
  else if (svg_.realOption(opt_name, opt_value, "pointsAtX", &r))
    pointsAtX_ = r;
  else if (svg_.realOption(opt_name, opt_value, "pointsAtY", &r))
    pointsAtY_ = r;
  else if (svg_.realOption(opt_name, opt_value, "pointsAtZ", &r))
    pointsAtZ_ = r;
  else if (svg_.realOption(opt_name, opt_value, "specularExponent", &r))
    specularExponent_ = r;
  else if (svg_.realOption(opt_name, opt_value, "limitingConeAngle", &r))
    limitingConeAngle_ = r;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeSpotLight::
draw()
{
}

void
CSVGFeSpotLight::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feSpotLight";

    CSVGObject::printValues(os);

    CSVGFilterBase::printValues(os);

    printNameValue(os, "x", x_);
    printNameValue(os, "y", y_);
    printNameValue(os, "z", z_);

    printNameValue(os, "pointsAtX", pointsAtX_);
    printNameValue(os, "pointsAtY", pointsAtY_);
    printNameValue(os, "pointsAtZ", pointsAtZ_);

    printNameValue(os, "specularExponent" , specularExponent_);
    printNameValue(os, "limitingConeAngle", limitingConeAngle_);

    os << "/>" << std::endl;
  }
  else
    os << "feSpotLight ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeSpotLight &fe)
{
  fe.print(os, false);

  return os;
}
