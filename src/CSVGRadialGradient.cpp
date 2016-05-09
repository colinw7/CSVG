#include <CSVGRadialGradient.h>
#include <CSVGLinearGradient.h>
#include <CSVGStop.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CMathGen.h>
#include <CSVGUtil.h>
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
 cx_        (rg.cx_        ),
 cy_        (rg.cy_        ),
 radius_    (rg.radius_    ),
 focusX_    (rg.focusX_    ),
 focusY_    (rg.focusY_    ),
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
  CMatrixStack2D  transform;

  if      (svg_.percentOption(opt_name, opt_value, "cx", length))
    cx_ = length;
  else if (svg_.percentOption(opt_name, opt_value, "cy", length))
    cy_ = length;
  else if (svg_.percentOption(opt_name, opt_value, "r" , length))
    radius_ = length;
  else if (svg_.coordOption(opt_name, opt_value, "fx", &real))
    focusX_ = real;
  else if (svg_.coordOption(opt_name, opt_value, "fy", &real))
    focusY_ = real;
  else if (svg_.transformOption(opt_name, opt_value, "gradientTransform", transform))
    setGTransform(transform);
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

    if (! decodeXLink(opt_value, &object, 0))
      return false;

    if  (object != 0) {
      CSVGRadialGradient *rg = dynamic_cast<CSVGRadialGradient *>(object);
      CSVGLinearGradient *lg = dynamic_cast<CSVGLinearGradient *>(object);

      if      (rg != 0) {
        if (rg->cx_    .isValid()) cx_     = rg->cx_;
        if (rg->cy_    .isValid()) cy_     = rg->cy_;
        if (rg->radius_.isValid()) radius_ = rg->radius_;
        if (rg->focusX_.isValid()) focusX_ = rg->focusX_;
        if (rg->focusY_.isValid()) focusY_ = rg->focusY_;

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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<radialGradient";

    CSVGObject::printValues(os);

    printNameLength(os, "cx", cx_    );
    printNameLength(os, "cy", cy_    );
    printNameLength(os, "r" , radius_);

    printNameTransform(os, "gradientTransform", gtransform_);

    if (getUnitsValid())
      os << " gradientUnits=\"" << CSVG::encodeUnitsString(getUnits()) << "\"";

    if (getSpreadValid())
      os << " spreadMethod=\"" << CSVG::encodeGradientSpread(getSpread()) << "\"";

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</radialGradient>" << std::endl;
  }
  else
    os << "radialGradient ";
}

CRadialGradient *
CSVGRadialGradient::
createGradient(CSVGObject *obj)
{
  CSVGBuffer *currentBuffer = svg_.getBuffer();

  CRadialGradient *gradient = new CRadialGradient;

  double xc = getCenterX();
  double yc = getCenterX();
  double r  = getRadius();
  double xf = getFocusX();
  double yf = getFocusY();

  //---

  // remap points to absolute
  if      (getUnits() == CSVGCoordUnits::USER_SPACE) {
    CMatrixStack2D m = currentBuffer->transform();

    // radius ?
    CPoint2D p1(xc, yc);
    CPoint2D p2(xf, yf);

    m.multiplyPoint(p1.x, p1.y, &xc, &yc);
    m.multiplyPoint(p2.x, p2.y, &xf, &yf);
  }
  else if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CBBox2D bbox;

    obj->getBBox(bbox);

    xc = CSVGUtil::map(xc, 0, 1, bbox.getXMin(), bbox.getXMax());
    yc = CSVGUtil::map(yc, 0, 1, bbox.getYMin(), bbox.getYMax());
    xf = CSVGUtil::map(xf, 0, 1, bbox.getXMin(), bbox.getXMax());
    yf = CSVGUtil::map(yf, 0, 1, bbox.getYMin(), bbox.getYMax());
    r  = CSVGUtil::map(r , 0, 1, 0, bbox.getWidth()); // w or h or both
  }

  //---

  gradient->setCenter(xc, yc);
  gradient->setRadius(r);
  gradient->setFocus (xf, yf);

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
