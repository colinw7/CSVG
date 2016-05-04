#include <CSVGFeLighting.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpotLight.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CRGBName.h>
#include <CVector3D.h>

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

void
CSVGFeLighting::
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
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

  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  for (const auto &l : lights) {
    CSVGFeDistantLight *dl = dynamic_cast<CSVGFeDistantLight *>(l);
    CSVGFePointLight   *pl = dynamic_cast<CSVGFePointLight   *>(l);
    CSVGFeSpotLight    *sl = dynamic_cast<CSVGFeSpotLight    *>(l);

    lightData_ = CSVGLightData();

    lightData_.ltype           = l->getObjTypeId();
    lightData_.lcolor          = getLightingColor();
    lightData_.surfaceScale    = getSurfaceScale();
    lightData_.isDiffuse       = isDiffuse();
    lightData_.diffuseConstant = getDiffuseConstant();
    lightData_.isSpecular      = isSpecular();
    lightData_.specConstant    = getSpecularConstant();
    lightData_.specExponent    = getSpecularExponent();

    if      (dl) distantLight(dst_image, dl);
    else if (pl) pointLight  (dst_image, pl);
    else if (sl) spotLight   (dst_image, sl);
  }

  outBuffer->setImage(dst_image);
}

void
CSVGFeLighting::
distantLight(CImagePtr image, CSVGFeDistantLight *light)
{
  lightData_.lelevation = light->getElevation();
  lightData_.lazimuth   = light->getAzimuth();

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1 = lightPoint(image, x, y);

      image->setRGBAPixel(x, y, rgba1);
    }
  }
}

void
CSVGFeLighting::
pointLight(CImagePtr image, CSVGFePointLight *light)
{
  lightData_.lpoint = light->getPoint();

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1 = lightPoint(image, x, y);

      image->setRGBAPixel(x, y, rgba1);
    }
  }
}

void
CSVGFeLighting::
spotLight(CImagePtr image, CSVGFeSpotLight *light)
{
  lightData_.lpoint    = light->getPoint();
  lightData_.lpointsAt = light->getPointsAt();
  lightData_.lexponent = light->getSpecularExponent();
  lightData_.lcone     = (light->hasLimitingConeAngle() ? cos(light->getLimitingConeAngle()) : 0);

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1 = lightPoint(image, x, y);

      image->setRGBAPixel(x, y, rgba1);
    }
  }
}

CRGBA
CSVGFeLighting::
lightPoint(CImagePtr image, int x, int y) const
{
  // calc light color
  double lx, ly, lz;

  if (lightData_.ltype == CSVGObjTypeId::FE_DISTANT_LIGHT) {
    lx = cos(lightData_.lazimuth)*cos(lightData_.lelevation);
    ly = sin(lightData_.lazimuth)*cos(lightData_.lelevation);
    lz = sin(lightData_.lelevation);
  }
  else {
    double gray;

    image->getGrayPixel(x, y, &gray);

    double z = lightData_.surfaceScale*gray;

    lx = lightData_.lpoint.x - x;
    ly = lightData_.lpoint.y - y;
    lz = lightData_.lpoint.z - z;
  }

  CVector3D lightDir = CVector3D(lx, ly, lz).unit();

  CRGBA lcolor;

  if (lightData_.ltype == CSVGObjTypeId::FE_SPOT_LIGHT) {
    CVector3D dspot(lightData_.lpoint, lightData_.lpointsAt);

    double ldot = lightDir.dotProduct(dspot.unit());

    if (ldot >= 0 && -ldot >= lightData_.lcone)
      lcolor = lightData_.lcolor*pow(-ldot, lightData_.lexponent);
    else
      lcolor = CRGBA(0,0,0);
  }
  else
    lcolor = lightData_.lcolor;

  //---

  // get gradient at point
  double xgray, ygray, xf, yf;

  image->sobelPixelGradient(x, y, 1, 1, xgray, ygray, xf, yf);

  // calc normal from gradient
  double nx = -lightData_.surfaceScale*xf*xgray;
  double ny = -lightData_.surfaceScale*yf*ygray;
  double nz = 1;

  CVector3D normal = CVector3D(nx, ny, nz).unit();

  //---

  if      (lightData_.isSpecular) {
    CVector3D eye(0, 0, 1);

    CVector3D halfway = (lightDir + eye).unit();

    //---

    //double dot = normal.dotProduct(lightDir);
    double dot = normal.dotProduct(halfway);

    CRGBA  scolor = lightData_.specConstant*pow(dot, lightData_.specExponent)*lcolor;
    double sa     = scolor.getIntensity();

    scolor.setAlpha(sa);

    return scolor.clamp();
  }
  else if (lightData_.isDiffuse) {
    double dot = normal.dotProduct(lightDir);

    CRGBA diffuse = lightData_.diffuseConstant*dot*lcolor;

    diffuse.setAlpha(1);

    return diffuse.clamp();
  }
  else
    return CRGBA();
}
