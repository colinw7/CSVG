#include <CSVGFeDiffuseLighting.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpotLight.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CRGBName.h>
#include <CVector3D.h>

CSVGFeDiffuseLighting::
CSVGFeDiffuseLighting(CSVG &svg) :
 CSVGFilterBase(svg)
{
}

CSVGFeDiffuseLighting::
CSVGFeDiffuseLighting(const CSVGFeDiffuseLighting &fe) :
 CSVGFilterBase(fe),
 filterIn_ (fe.filterIn_),
 filterOut_(fe.filterOut_)
{
}

CSVGFeDiffuseLighting *
CSVGFeDiffuseLighting::
dup() const
{
  return new CSVGFeDiffuseLighting(*this);
}

bool
CSVGFeDiffuseLighting::
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
  else if (svg_.realOption(opt_name, opt_value, "surfaceScale", &r))
    surfaceScale_ = r;
  else if (svg_.realOption(opt_name, opt_value, "diffuseConstant", &r))
    diffuseConstant_ = r;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeDiffuseLighting::
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
CSVGFeDiffuseLighting::
filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer)
{
  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  for (const auto &c : children()) {
    CSVGFeDistantLight *dl = dynamic_cast<CSVGFeDistantLight *>(c);
    CSVGFePointLight   *pl = dynamic_cast<CSVGFePointLight   *>(c);
    CSVGFeSpotLight    *sl = dynamic_cast<CSVGFeSpotLight   *>(c);

    if      (dl) distantLight(dst_image, dl);
    else if (pl) pointLight  (dst_image, pl);
    else if (sl) spotLight   (dst_image, sl);
  }

  outBuffer->setImage(dst_image);
}

void
CSVGFeDiffuseLighting::
distantLight(CImagePtr /*image*/, CSVGFeDistantLight *)
{
  // TODO
#if 0
  lpoint_ = pl->getPoint();
  lcolor_ = getLightingColor();

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba;

      image->getRGBAPixel(x, y, rgba);

      CRGBA rgba1 = lightPoint(rgba, CPoint3D(x, y, 0));

      image->setRGBAPixel(x, y, rgba1);
    }
  }
#endif
}

void
CSVGFeDiffuseLighting::
pointLight(CImagePtr image, CSVGFePointLight *pl)
{
  lpoint_ = pl->getPoint();
  lcolor_ = getLightingColor();

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba;

      image->getRGBAPixel(x, y, rgba);

      CRGBA rgba1 = lightPoint(rgba, CPoint3D(x, y, 0));

      image->setRGBAPixel(x, y, rgba1);
    }
  }
}

void
CSVGFeDiffuseLighting::
spotLight(CImagePtr /*image*/, CSVGFeSpotLight *)
{
  // TODO
#if 0
  lpoint_ = pl->getPoint();
  lcolor_ = getLightingColor();

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba;

      image->getRGBAPixel(x, y, rgba);

      CRGBA rgba1 = lightPoint(rgba, CPoint3D(x, y, 0));

      image->setRGBAPixel(x, y, rgba1);
    }
  }
#endif
}

CRGBA
CSVGFeDiffuseLighting::
lightPoint(CRGBA &rgba, const CPoint3D &point) const
{
  CVector3D normal(0, 0, 1);

  // Ambient
  //CRGBA ambient = getAmbient()*material.getAmbient();
  //CRGBA ambient(0,0,0);

  // Diffuse
  CVector3D dlight(point, lpoint_);

  dlight.normalize();

  // uncomment if light both sides
  //double dot = fabs(dlight.dotProduct(normal));
  double dot = dlight.dotProduct(normal);

  if (dot < 0.0)
    dot = 0.0;

  //CRGBA diffuse = dot*getDiffuse()*material.getDiffuse();
  CRGBA diffuse = dot*lcolor_*rgba;

#if 0
  // Specular
  CRGBA specular(0,0,0,1);

  if (dot > 0.0) {
    CVector3D viewpoint(0,0,1);

    CVector3D sum(viewpoint + dlight);

    sum.normalize();

    double dot1 = sum.dotProduct(normal);

    if (dot1 < 0.0)
      dot1 = 0.0;

    specular = pow(dot1, material.getShininess())*getSpecular()*material.getSpecular();
  }

  double dist = CVector3D(point, lpoint_).length();

  rgba += getAttenuation(dist)*getSpotEffect(point)*(ambient + diffuse + specular);
#endif

  CRGBA rgba1 = diffuse;

  //rgba1.setAlpha(material.getDiffuse().getAlpha());
  rgba1.setAlpha(rgba.getAlpha());

  return rgba1;
}

void
CSVGFeDiffuseLighting::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feDiffuseLighting";

    CSVGObject::printValues(os);

    printNameValue(os, "in"             , filterIn_ );
    printNameValue(os, "result"         , filterOut_);
    printNameValue(os, "lighting-color" , lightingColor_);
    printNameValue(os, "surfaceScale"   , surfaceScale_);
    printNameValue(os, "diffuseConstant", diffuseConstant_);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</feDiffuseLighting>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "feDiffuseLighting ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeDiffuseLighting &fe)
{
  fe.print(os, false);

  return os;
}
