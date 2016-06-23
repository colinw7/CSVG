#include <CSVGFeLighting.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpotLight.h>
#include <CSVGBuffer.h>
#include <CSVGFilter.h>
#include <CSVG.h>

CSVGFeLighting::
CSVGFeLighting(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeLighting::
CSVGFeLighting(const CSVGFeLighting &fe) :
 CSVGFilterBase   (fe),
 filterIn_        (fe.filterIn_),
 filterOut_       (fe.filterOut_),
 lightingColor_   (fe.lightingColor_),
 diffuseConstant_ (fe.diffuseConstant_),
 specularConstant_(fe.specularConstant_),
 specularExponent_(fe.specularExponent_),
 surfaceScale_    (fe.surfaceScale_)
{
}

std::string
CSVGFeLighting::
getFilterIn() const
{
  return calcFilterIn(filterIn_);
}

std::string
CSVGFeLighting::
getFilterOut() const
{
  return calcFilterOut(filterOut_);
}

void
CSVGFeLighting::
filterImage(CSVGBuffer *inBuffer, const CBBox2D &bbox, CSVGBuffer *outBuffer)
{
  std::vector<CSVGFilterBase *> lights;

  for (const auto &c : children()) {
    CSVGFeDistantLight *dl = dynamic_cast<CSVGFeDistantLight *>(c);
    CSVGFePointLight   *pl = dynamic_cast<CSVGFePointLight   *>(c);
    CSVGFeSpotLight    *sl = dynamic_cast<CSVGFeSpotLight    *>(c);

    if      (dl) lights.push_back(dl);
    else if (pl) lights.push_back(pl);
    else if (sl) lights.push_back(sl);
  }

  CSVGLightData lightData;

  lightData.lcolor          = svg_.colorToRGBA(getLightingColor());
  lightData.surfaceScale    = getSurfaceScale();
  lightData.isDiffuse       = isDiffuse();
  lightData.diffuseConstant = getDiffuseConstant();
  lightData.isSpecular      = isSpecular();
  lightData.specConstant    = getSpecularConstant();
  lightData.specExponent    = getSpecularExponent();

  CSVGBuffer::lightBuffers(inBuffer, bbox, lights, lightData, outBuffer);
}
