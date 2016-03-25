#include <CSVGRadialGradient.h>
#include <CSVGLinearGradient.h>
#include <CSVGStop.h>
#include <CSVG.h>
#include <CMathGen.h>
#include <CRadialGradient.h>

/* Attributes:
    <Core>
    <Style>
    <Color>
    <Gradient>
    <XLink>
    <External>
    cx <Coordinate>
    cy <Coordinate>
    r <Length>
    fx <Coordinate>
    fy <Coordinate>
    gradientUnits ( userSpaceOnUse | objectBoundingBox )
    gradientTransform <TransformList>
    spreadMethod ( pad | reflect | repeat )
*/

CSVGRadialGradient::
CSVGRadialGradient(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGRadialGradient::
CSVGRadialGradient(const CSVGRadialGradient &rg) :
 CSVGObject (rg),
 center_    (rg.center_    ),
 radius_    (rg.radius_    ),
 focus_     (rg.focus_     ),
 stops_     (rg.stops_     ),
 gtransform_(rg.gtransform_),
 units_     (rg.units_     ),
 spread_    (rg.spread_    )
{
}

CSVGRadialGradient *
CSVGRadialGradient::
dup() const
{
  return new CSVGRadialGradient(*this);
}

bool
CSVGRadialGradient::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string     str;
  double          real;
  CSVGCoordUnits  units;
  CSVGLengthValue length;

  if      (svg_.percentOption(opt_name, opt_value, "cx", length)) {
    setCenterX(real);
  }
  else if (svg_.percentOption(opt_name, opt_value, "cy", length)) {
    setCenterY(real);
  }
  else if (svg_.percentOption(opt_name, opt_value, "r" , length)) {
    setRadius(real);
  }
  else if (svg_.coordOption(opt_name, opt_value, "fx", &real)) {
    setFocusX(real);
  }
  else if (svg_.coordOption(opt_name, opt_value, "fy", &real)) {
    setFocusY(real);
  }
  else if (svg_.stringOption(opt_name, opt_value, "gradientTransform", str)) {
    CMatrix2D gtransform;

    gtransform.setIdentity();

    if (! svg_.decodeTransform(str, gtransform))
      return false;

    setGTransform(gtransform);
  }
  else if (svg_.coordUnitsOption(opt_name, opt_value, "gradientUnits", units)) {
    setUnits(units);
  }
  else if (svg_.stringOption(opt_name, opt_value, "spreadMethod", str)) {
    CGradientSpreadType spread;

    if (! svg_.decodeGradientSpread(str, spread))
      return false;

    setSpread(spread);
  }
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str)) {
    CSVGObject *object;
    CImagePtr   image;

    if (! decodeXLink(opt_value, &object, image))
      return false;

    if  (object != 0) {
      CSVGRadialGradient *rg = dynamic_cast<CSVGRadialGradient *>(object);
      CSVGLinearGradient *lg = dynamic_cast<CSVGLinearGradient *>(object);

      if      (rg != 0) {
        if (rg->center_.isValid()) center_ = rg->center_;
        if (rg->radius_.isValid()) radius_ = rg->radius_;
        if (rg->focus_ .isValid()) focus_  = rg->focus_;

        if (rg->getGTransformValid()) gtransform_ = rg->getGTransform();
        if (rg->getUnitsValid     ()) units_      = rg->getUnits();
        if (rg->getSpreadValid    ()) spread_     = rg->getSpread();

        if (rg->anyStops()) {
          stops_.clear();

          for (const auto &s : rg->stops())
            addStop(s);
        }
      }
      else if (lg != 0) {
        if (lg->getGTransformValid()) gtransform_ = lg->getGTransform();
        if (lg->getUnitsValid     ()) units_      = lg->getUnits();
        if (lg->getSpreadValid    ()) spread_     = lg->getSpread();

        if (lg->anyStops()) {
          stops_.clear();

          for (const auto &s : lg->stops())
            addStop(s);
        }
      }
    }
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGRadialGradient::
termParse()
{
  std::vector<CSVGObject *> objects;

  getChildrenOfType(CSVGObjTypeId::STOP, objects);

  for (const auto &o : objects) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(o);

    stops_.push_back(stop);
  }
}

void
CSVGRadialGradient::
draw()
{
}

void
CSVGRadialGradient::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<radialGradient";

    printNameValue(os, "id", id_);

    if (center_.isValid())
      os << " cx=\"" << center_.getValue().x << "\" cy=\"" << center_.getValue().y << "\"";

    printNameValue(os, "r", radius_);

    if (gtransform_.isValid() && ! gtransform_.getValue().isIdentity()) {
      os << " gradientTransform=\"";

      printTransform(os, gtransform_.getValue());

      os << "\"";
    }

    if (getUnitsValid())
      os << " gradientUnits=\"" << CSVG::encodeUnitsString(getUnits()) << "\"";

    if (getSpreadValid())
      os << " spreadMethod=\"" << CSVG::encodeGradientSpread(getSpread()) << "\"";

    os << ">" << std::endl;

    for (const auto &o : objects_)
      o->print(os, hier);

    os << "</radialGradient>" << std::endl;
  }
  else
    os << "radialGradient ";
}

CImagePtr
CSVGRadialGradient::
getImage(CBBox2D &bbox)
{
  CImageNameSrc src("CSVG/CSVGRadialGradient::getImage");

  CImagePtr image = CImageMgrInst->createImage(src);

  double width  = bbox.getWidth ();
  double height = bbox.getHeight();

  int pwidth, pheight;

  svg_.lengthToPixel(width, height, &pwidth, &pheight);

  image->setDataSize(pwidth, pheight);

  CRadialGradient gradient;

  int pxc, pyc, prx, pry, pxf, pyf;

  svg_.windowToPixel(getCenterX(), getCenterY(), &pxc, &pyc);
  svg_.lengthToPixel(getRadius(), getRadius(), &prx, &pry);

  if (focus_.isValid())
    svg_.windowToPixel(getFocusX(), getFocusY(), &pxf, &pyf);
  else
    svg_.windowToPixel(getCenterX(), getCenterY(), &pxf, &pyf);

  gradient.setCenter(pxc, pyc);
  gradient.setRadius(std::max(prx, pry));
  gradient.setFocus (pxf, pyf);

  gradient.setSpread(getSpread());

  for (const auto &s : stops())
    gradient.addStop(s->getOffset(), s->getColor());

  image->radialGradient(gradient);

  return image;
}

CRadialGradient *
CSVGRadialGradient::
createGradient()
{
  CRadialGradient *gradient = new CRadialGradient;

  if (center_.isValid())
    gradient->setCenter(getCenterX(), getCenterY());
  else
    gradient->setCenter(0.5, 0.5);

  if (radius_.isValid())
    gradient->setRadius(getRadius());
  else
    gradient->setRadius(0.5);

  if (focus_.isValid())
    gradient->setFocus(getFocusX(), getFocusY());
  else if (center_.isValid())
    gradient->setFocus(getCenterX(), getCenterY());
  else
    gradient->setFocus(0.5, 0.5);

  if (spread_.isValid())
    gradient->setSpread(getSpread());

  for (const auto &s : stops()) {
    CRGBA  rgba  = s->getColor ();
    double alpha = s->getOpacity();

    rgba.setAlpha(alpha);

    gradient->addStop(s->getOffset(), rgba);
  }

  gradient->init(1, 1);

  return gradient;
}

std::ostream &
operator<<(std::ostream &os, const CSVGRadialGradient &gradient)
{
  gradient.print(os, false);

  return os;
}
