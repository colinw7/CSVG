#include <CSVGLinearGradient.h>
#include <CSVGRadialGradient.h>
#include <CSVGStop.h>
#include <CSVG.h>
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
 CSVGObject(svg)
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string     str;
  CSVGCoordUnits  units;
  CSVGLengthValue length;

  if      (svg_.percentOption(opt_name, opt_value, "x1", length)) {
    setX1(length.value());
  }
  else if (svg_.percentOption(opt_name, opt_value, "y1", length)) {
    setY1(length.value());
  }
  else if (svg_.percentOption(opt_name, opt_value, "x2", length)) {
    setX2(length.value());
  }
  else if (svg_.percentOption(opt_name, opt_value, "y2", length)) {
    setY2(length.value());
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

      if      (lg != 0) {
        if (lg->x1_.isValid()) x1_ = lg->x1_;
        if (lg->y1_.isValid()) y1_ = lg->y1_;
        if (lg->x2_.isValid()) x2_ = lg->x2_;
        if (lg->y2_.isValid()) y2_ = lg->y2_;

        if (lg->getGTransformValid()) gtransform_ = lg->getGTransform();
        if (lg->getUnitsValid     ()) units_      = lg->getUnits();
        if (lg->getSpreadValid    ()) spread_     = lg->getSpread();

        if (lg->anyStops()) {
          stops_.clear();

          for (const auto &s : lg->stops())
            addStop(s);
        }
      }
      else if (rg != 0) {
        if (rg->getGTransformValid()) gtransform_ = rg->getGTransform();
        if (rg->getUnitsValid     ()) units_      = rg->getUnits();
        if (rg->getSpreadValid    ()) spread_     = rg->getSpread();

        if (rg->anyStops()) {
          stops_.clear();

          for (const auto &s : rg->stops())
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
CSVGLinearGradient::
termParse()
{
  std::vector<CSVGObject *> objects;

  getChildrenOfType(CSVGObjTypeId::STOP, objects);

  for (const auto &o : objects) {
    CSVGStop *stop = dynamic_cast<CSVGStop *>(o);

    if (stop != 0)
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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<linearGradient";

    CSVGObject::printValues(os);

    printNameValue(os, "x1", x1_);
    printNameValue(os, "y1", y1_);
    printNameValue(os, "x2", x2_);
    printNameValue(os, "y2", y2_);

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

    printChildren(os, hier);

    os << "</linearGradient>" << std::endl;
  }
  else
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

  for (const auto &s : stops())
    gradient.addStop(s->getOffset(), s->getColor());

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

  for (const auto &s : stops())
    gradient->addStop(s->getOffset(), s->getColor());

  gradient->init(1, 1);

  return gradient;
}

std::ostream &
operator<<(std::ostream &os, const CSVGLinearGradient &gradient)
{
  gradient.print(os, false);

  return os;
}
