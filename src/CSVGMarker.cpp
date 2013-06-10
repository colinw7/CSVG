#include "CSVGI.h"

/* Attributes:
    <Core>
    <Style>
    <Presentation>
    <External>
    refX <Coordinate>
    refY <Coordinate>
    markerUnits ( strokeWidth | userSpaceOnUse ) #IMPLIED
    markerWidth  <Length>
    markerHeight <Length>
    orient
    viewBox <ViewBoxSpec>
    preserveAspectRatio <PreserveAspectRatioSpec>
*/

CSVGMarker::
CSVGMarker(CSVG &svg) :
 CSVGObject   (svg),
 refX_        (0),
 refY_        (0),
 markerWidth_ (1),
 markerHeight_(1)
{
}

CSVGMarker::
CSVGMarker(const CSVGMarker &marker) :
 CSVGObject   (marker),
 refX_        (marker.refX_),
 refY_        (marker.refY_),
 markerWidth_ (marker.markerWidth_),
 markerHeight_(marker.markerHeight_)
{
}

CSVGMarker *
CSVGMarker::
dup() const
{
  return new CSVGMarker(*this);
}

bool
CSVGMarker::
processOption(const string &opt_name, const string &opt_value)
{
  if (processCoreOption        (opt_name, opt_value)) return true;
  if (processStyleOption       (opt_name, opt_value)) return true;
  if (processPresentationOption(opt_name, opt_value)) return true;
  if (processExternalOption    (opt_name, opt_value)) return true;

  string  str;
  double  real;
  CBBox2D bbox;

  if      (svg_.coordOption (opt_name, opt_value, "refX", &real))
    refX_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "refY", &real))
    refY_ = real;
  else if (svg_.stringOption(opt_name, opt_value, "markerUnits", str))
    ;
  else if (svg_.lengthOption(opt_name, opt_value, "markerWidth", &real) ||
           svg_.lengthOption(opt_name, opt_value, "marker-width", &real))
    markerWidth_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "markerHeight", &real) ||
           svg_.lengthOption(opt_name, opt_value, "marker-height", &real))
    markerHeight_ = real;
  else if (svg_.stringOption(opt_name, opt_value, "orient", str))
    ;
  else if (svg_.bboxOption  (opt_name, opt_value, "viewBox", &bbox))
    view_box_ = bbox;
  else if (svg_.stringOption(opt_name, opt_value, "preserveAspectRatio", str)) {
    //if (! svg_.decodePreserveAspectRatio(opt_value, &halign_, &valign_, &scale_))
    //  return false;
  }
  else
    return false;

  return true;
}

void
CSVGMarker::
draw()
{
}

void
CSVGMarker::
drawMarker(double x, double y, double angle)
{
  CBBox2D bbox;

  getChildrenBBox(bbox);

  double x1 = 0, y1 = 0, w1 = 1, h1 = 1;

  if (bbox.isSet()) {
    x1 = bbox.getXMin();
    y1 = bbox.getYMin();
    w1 = bbox.getWidth ();
    h1 = bbox.getHeight();
  }

  double xm = x1 + w1/2;
  double ym = y1 + h1/2;

  CMatrix2D matrix1, matrix2, matrix3, matrix4, matrix5;

  matrix1.setTranslation(x - refX_, y - refY_);

  matrix2.setScale(markerWidth_/w1, markerHeight_/h1);

  matrix3.setTranslation(w1/2, h1/2);

  matrix4.setRotation(-angle);

  matrix5.setTranslation(-xm, -ym);

  CMatrix2D transform;

  svg_.getTransform(transform);

  svg_.setTransform(transform*matrix1*matrix2*matrix3*matrix4*matrix5);

  drawObjects();

  svg_.setTransform(transform);
}

void
CSVGMarker::
print(ostream &os) const
{
  os << "marker";
}

ostream &
operator<<(ostream &os, const CSVGMarker &marker)
{
  marker.print(os);

  return os;
}
