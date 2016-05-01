#include <CSVGFeSpecularLighting.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpotLight.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CRGBName.h>
#include <CVector3D.h>

CSVGFeSpecularLighting::
CSVGFeSpecularLighting(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeSpecularLighting::
CSVGFeSpecularLighting(const CSVGFeSpecularLighting &fe) :
 CSVGFilterBase(fe),
 filterIn_        (fe.filterIn_),
 filterOut_       (fe.filterOut_),
 lightingColor_   (fe.lightingColor_),
 specularConstant_(fe.specularConstant_),
 specularExponent_(fe.specularExponent_),
 surfaceScale_    (fe.surfaceScale_)
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
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
{
  specConstant_ = getSpecularConstant();
  specExponent_ = getSpecularExponent();

  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  for (const auto &c : children()) {
    CSVGFeDistantLight *dl = dynamic_cast<CSVGFeDistantLight *>(c);
    CSVGFePointLight   *pl = dynamic_cast<CSVGFePointLight   *>(c);
    CSVGFeSpotLight    *sl = dynamic_cast<CSVGFeSpotLight    *>(c);

    if      (dl) distantLight(dst_image, dl);
    else if (pl) pointLight  (dst_image, pl);
    else if (sl) spotLight   (dst_image, sl);
  }

  outBuffer->setImage(dst_image);
}

void
CSVGFeSpecularLighting::
distantLight(CImagePtr image, CSVGFeDistantLight *light)
{
  ltype_        = light->getObjTypeId();
  lcolor_       = getLightingColor();
  lelevation_   = light->getElevation();
  lazimuth_     = light->getAzimuth();
  lpoint_       = CPoint3D();
  lpointsAt_    = CPoint3D();
  lexponent_    = 0;
  lcone_        = 0;

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
CSVGFeSpecularLighting::
pointLight(CImagePtr image, CSVGFePointLight *light)
{
  ltype_      = light->getObjTypeId();
  lpoint_     = light->getPoint();
  lcolor_     = getLightingColor();
  lpointsAt_  = CPoint3D();
  lelevation_ = 0;
  lazimuth_   = 0;
  lexponent_  = 0;
  lcone_      = 0;

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
CSVGFeSpecularLighting::
spotLight(CImagePtr image, CSVGFeSpotLight *light)
{
  ltype_      = light->getObjTypeId();
  lpoint_     = light->getPoint();
  lcolor_     = getLightingColor();
  lpointsAt_  = light->getPointsAt();
  lexponent_  = light->getSpecularExponent();
  lcone_      = (light->hasLimitingConeAngle() ? cos(light->getLimitingConeAngle()) : 0);
  lelevation_ = 0;
  lazimuth_   = 0;

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
CSVGFeSpecularLighting::
lightPoint(CImagePtr image, int x, int y) const
{
  // calc light color
  double lx, ly, lz;

  if (ltype_ == CSVGObjTypeId::FE_DISTANT_LIGHT) {
    lx = cos(lazimuth_)*cos(lelevation_);
    ly = sin(lazimuth_)*cos(lelevation_);
    lz = sin(lelevation_);
  }
  else {
    double gray;

    image->getGrayPixel(x, y, &gray);

    double z = getSurfaceScale()*gray;

    lx = lpoint_.x - x;
    ly = lpoint_.y - y;
    lz = lpoint_.z - z;
  }

  CVector3D lightDir = CVector3D(lx, ly, lz).unit();

  CRGBA lcolor;

  if (ltype_ == CSVGObjTypeId::FE_SPOT_LIGHT) {
    CVector3D dspot(lpoint_, lpointsAt_);

    double ldot = lightDir.dotProduct(dspot.unit());

    if (ldot >= 0 && -ldot >= lcone_)
      lcolor = lcolor_*pow(-ldot, lexponent_);
    else
      lcolor = CRGBA(0,0,0);
  }
  else
    lcolor = lcolor_;

  //---

  // get gradient at point
  double xgray, ygray, xf, yf;

  image->sobelPixelGradient(x, y, 1, 1, xgray, ygray, xf, yf);

  // calc normal from gradient
  double nx = -getSurfaceScale()*xf*xgray;
  double ny = -getSurfaceScale()*yf*ygray;
  double nz = 1;

  CVector3D normal = CVector3D(nx, ny, nz).unit();

  CVector3D eye(0, 0, 1);

  CVector3D halfway = (lightDir + eye).unit();

  //---

  //double dot = normal.dotProduct(lightDir);
  double dot = normal.dotProduct(halfway);

  CRGBA  scolor = specConstant_*pow(dot, specExponent_)*lcolor;
  double sa     = scolor.getIntensity();

  scolor.setAlpha(sa);

  return scolor.clamp();
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
