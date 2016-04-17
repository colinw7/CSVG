#include <CSVGFeSpecularLighting.h>
#include <CSVGFePointLight.h>
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
  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  for (const auto &c : children()) {
    CSVGFePointLight *pl = dynamic_cast<CSVGFePointLight *>(c);
    if (! pl) continue;

    pointLight(dst_image, pl);
  }

  outBuffer->setImage(dst_image);
}

void
CSVGFeSpecularLighting::
pointLight(CImagePtr image, CSVGFePointLight *pl)
{
  lpoint_ = pl->getPoint();
  lcolor_ = getLightingColor();

  specConstant_ = getSpecularConstant();
  specExponent_ = getSpecularExponent();

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

CRGBA
CSVGFeSpecularLighting::
lightPoint(CRGBA &rgba, const CPoint3D &point) const
{
  CVector3D normal(0, 0, 1);

  // Light vector
  CVector3D dlight(point, lpoint_);

  dlight.normalize();

  // normal.light
  double dot = dlight.dotProduct(normal);

  if (dot < 0.0)
    dot = 0.0;

  double sr = specConstant_*pow(dot, specExponent_)*rgba.getRed  ();
  double sg = specConstant_*pow(dot, specExponent_)*rgba.getGreen();
  double sb = specConstant_*pow(dot, specExponent_)*rgba.getBlue ();
  double sa = std::max(sr, std::max(sg, sb));

  return CRGBA(sr, sg, sb, sa).clamp();
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
