#include "CSVGI.h"
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
 CSVGObject (svg),
 center_    (),
 radius_    (),
 focus_     (),
 stops_     (),
 gtransform_(),
 units_     (),
 spread_    ()
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
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;

  if      (svg_.stringOption(opt_name, opt_value, "cx", str)) {
    if (! svg_.decodePercentString(str, &real))
      return false;

    setCenterX(real);
  }
  else if (svg_.stringOption(opt_name, opt_value, "cy", str)) {
    if (! svg_.decodePercentString(str, &real))
      return false;

    setCenterY(real);
  }
  else if (svg_.stringOption(opt_name, opt_value, "r" , str)) {
    if (! svg_.decodePercentString(str, &real))
      return false;

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
  else if (svg_.stringOption(opt_name, opt_value, "gradientUnits", str)) {
    CSVGCoordUnits units;

    if (! svg_.decodeUnitsString(str, &units))
      return false;

    setUnits(units);
  }
  else if (svg_.stringOption(opt_name, opt_value, "spreadMethod", str)) {
    CGradientSpreadType spread;

    if (! svg_.decodeGradientSpread(str, &spread))
      return false;

    setSpread(spread);
  }
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str)) {
    CSVGObject *object;
    CImagePtr   image;

    if (! decodeXLink(opt_value, &object, image))
      return false;

    if  (object != NULL) {
      CSVGRadialGradient *rg = dynamic_cast<CSVGRadialGradient *>(object);
      CSVGLinearGradient *lg = dynamic_cast<CSVGLinearGradient *>(object);

      if      (rg != NULL) {
        if (rg->center_.isValid()) center_ = rg->center_;
        if (rg->radius_.isValid()) radius_ = rg->radius_;
        if (rg->focus_ .isValid()) focus_  = rg->focus_;

        if (rg->getGTransformValid()) gtransform_ = rg->getGTransform();
        if (rg->getUnitsValid     ()) units_      = rg->getUnits();
        if (rg->getSpreadValid    ()) spread_     = rg->getSpread();

        if (rg->anyStops()) {
          stops_.clear();

          StopList::const_iterator p1 = rg->beginStops();
          StopList::const_iterator p2 = rg->endStops  ();

          for ( ; p1 != p2; ++p1)
            addStop(*p1);
        }
      }
      else if (lg != NULL) {
        if (lg->getGTransformValid()) gtransform_ = lg->getGTransform();
        if (lg->getUnitsValid     ()) units_      = lg->getUnits();
        if (lg->getSpreadValid    ()) spread_     = lg->getSpread();

        if (lg->anyStops()) {
          stops_.clear();

          StopList::const_iterator p1 = lg->beginStops();
          StopList::const_iterator p2 = lg->endStops  ();

          for ( ; p1 != p2; ++p1)
            addStop(*p1);
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
  vector<CSVGObject *> objects;

  getChildrenOfType(CSVG_OBJ_TYPE_STOP, objects);

  vector<CSVGObject *>::iterator p1 = objects.begin();
  vector<CSVGObject *>::iterator p2 = objects.end  ();

  for ( ; p1 != p2; ++p1) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(*p1);

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
print(ostream &os) const
{
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

  StopList::const_iterator p1 = stops_.begin();
  StopList::const_iterator p2 = stops_.end  ();

  for ( ; p1 != p2; ++p1)
    gradient.addStop((*p1)->getOffset(), (*p1)->getColor());

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

  StopList::const_iterator p1 = beginStops();
  StopList::const_iterator p2 = endStops  ();

  for ( ; p1 != p2; ++p1) {
    CRGBA rgba = (*p1)->getColor();

    double alpha = (*p1)->getOpacity();

    rgba.setAlpha(alpha);

    gradient->addStop((*p1)->getOffset(), rgba);
  }

  gradient->init(1, 1);

  return gradient;
}

ostream &
operator<<(ostream &os, const CSVGRadialGradient &gradient)
{
  gradient.print(os);

  return os;
}
