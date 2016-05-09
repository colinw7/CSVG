#include <CSVGFeSpecularLighting.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpotLight.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CRGBName.h>

CSVGFeSpecularLighting::
CSVGFeSpecularLighting(CSVG &svg) :
 CSVGFeLighting(svg)
{
}

CSVGFeSpecularLighting::
CSVGFeSpecularLighting(const CSVGFeSpecularLighting &fe) :
 CSVGFeLighting(fe)
{
}

CSVGFeSpecularLighting *
CSVGFeSpecularLighting::
dup() const
{
  return new CSVGFeSpecularLighting(*this);
}

bool
CSVGFeSpecularLighting::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  double      r;
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filterIn_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "lighting-color", str))
    lightingColor_ = CRGBName::toRGBA(str);
  else if (svg_.realOption(opt_name, opt_value, "specularConstant", &r))
    specularConstant_ = r;
  else if (svg_.realOption(opt_name, opt_value, "specularExponent", &r))
    specularExponent_ = r;
  else if (svg_.realOption(opt_name, opt_value, "surfaceScale", &r))
    surfaceScale_ = r;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeSpecularLighting::
draw()
{
  CSVGBuffer *inBuffer  = svg_.getBuffer(getFilterIn ());
  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImage(inBuffer->getImage());
  }

  filterImage(inBuffer, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
  }
}

void
CSVGFeSpecularLighting::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feSpecularLighting";

    CSVGObject::printValues(os);

    printNameValue(os, "in"              , filterIn_ );
    printNameValue(os, "result"          , filterOut_);
    printNameValue(os, "lighting-color"  , lightingColor_);
    printNameValue(os, "specularConstant", specularConstant_);
    printNameValue(os, "specularExponent", specularExponent_);
    printNameValue(os, "surfaceScale"    , surfaceScale_);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</feSpecularLighting>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "feSpecularLighting ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeSpecularLighting &fe)
{
  fe.print(os, false);

  return os;
}
