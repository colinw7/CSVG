#include <CSVGI.h>
#include <CLinearGradient.h>

/* Attributes:
    <Core>
    <Style>
    <Color>
    <Gradient>
    <XLink>
    <External>
    x1 <Coordinate>
    y1 <Coordinate>
    x2 <Coordinate>
    y2 <Coordinate>
    gradientUnits ( userSpaceOnUse | objectBoundingBox )
    gradientTransform <TransformList>
    spreadMethod ( pad | reflect | repeat )
*/

CSVGLinearGradient::
CSVGLinearGradient(CSVG &svg) :
 CSVGObject (svg),
 x1_        (),
 y1_        (),
 x2_        (),
 y2_        (),
 stops_     (),
 gtransform_(),
 units_     (),
 spread_    ()
{
  //x1_.setDefValue(0.0);
  //y1_.setDefValue(0.0);
  //x2_.setDefValue(1.0);
  //y2_.setDefValue(0.0);
}

CSVGLinearGradient::
CSVGLinearGradient(const CSVGLinearGradient &lg) :
 CSVGObject (lg),
 x1_        (lg.x1_),
 y1_        (lg.y1_),
 x2_        (lg.x2_),
 y2_        (lg.y2_),
 stops_     (lg.stops_),
 gtransform_(lg.gtransform_),
 units_     (lg.units_),
 spread_    (lg.spread_)
{
}

CSVGLinearGradient *
CSVGLinearGradient::
dup() const
{
  return new CSVGLinearGradient(*this);
}

bool
CSVGLinearGradient::
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;

  if      (svg_.stringOption(opt_name, opt_value, "x1", str)) {
    if (! svg_.decodePercentString(str, &real))
      return false;

    setX1(real);
  }
  else if (svg_.stringOption(opt_name, opt_value, "y1", str)) {
    if (! svg_.decodePercentString(str, &real))
      return false;

    setY1(real);
  }
  else if (svg_.stringOption(opt_name, opt_value, "x2", str)) {
    if (! svg_.decodePercentString(str, &real))
      return false;

    setX2(real);
  }
  else if (svg_.stringOption(opt_name, opt_value, "y2", str)) {
    if (! svg_.decodePercentString(str, &real))
      return false;

    setY2(real);
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

      if      (lg != NULL) {
        if (lg->x1_.isValid()) x1_ = lg->x1_;
        if (lg->y1_.isValid()) y1_ = lg->y1_;
        if (lg->x2_.isValid()) x2_ = lg->x2_;
        if (lg->y2_.isValid()) y2_ = lg->y2_;

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
      else if (rg != NULL) {
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
    }
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGLinearGradient::
termParse()
{
  vector<CSVGObject *> objects;

  getChildrenOfType(CSVG_OBJ_TYPE_STOP, objects);

  vector<CSVGObject *>::iterator p1 = objects.begin();
  vector<CSVGObject *>::iterator p2 = objects.end  ();

  for ( ; p1 != p2; ++p1) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(*p1);

    if (stop != NULL)
      addStop(stop);
  }
}

void
CSVGLinearGradient::
draw()
{
}

void
CSVGLinearGradient::
print(ostream &os) const
{
  os << "linearGradient ";
}

CImagePtr
CSVGLinearGradient::
getImage(CBBox2D &bbox)
{
  CImageNameSrc src("CSVG/CSVGLinearGradient::getImage");

  CImagePtr image = CImageMgrInst->createImage(src);

  double width  = bbox.getWidth ();
  double height = bbox.getHeight();

  int pwidth, pheight;

  svg_.lengthToPixel(width, height, &pwidth, &pheight);

  image->setDataSize(pwidth, pheight);

  CLinearGradient gradient;

  int px1, py1, px2, py2;

  svg_.windowToPixel(getXMin(), getYMin(), &px1, &py1);
  svg_.windowToPixel(getXMax(), getYMax(), &px2, &py2);

  gradient.setLine(px1, py1, px2, py2);

  gradient.setSpread(getSpread());

  StopList::const_iterator p1 = stops_.begin();
  StopList::const_iterator p2 = stops_.end  ();

  for ( ; p1 != p2; ++p1)
    gradient.addStop((*p1)->getOffset(), (*p1)->getColor());

  image->linearGradient(gradient);

  return image;
}

CLinearGradient *
CSVGLinearGradient::
createGradient()
{
  CLinearGradient *gradient = new CLinearGradient;

  gradient->setLine(getX1(), getY1(), getX2(), getY2());

  if (spread_.isValid())
    gradient->setSpread(getSpread());

  StopList::const_iterator p1 = beginStops();
  StopList::const_iterator p2 = endStops  ();

  for ( ; p1 != p2; ++p1)
    gradient->addStop((*p1)->getOffset(), (*p1)->getColor());

  gradient->init(1, 1);

  return gradient;
}

ostream &
operator<<(ostream &os, const CSVGLinearGradient &gradient)
{
  gradient.print(os);

  return os;
}
