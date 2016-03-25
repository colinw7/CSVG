#include <CSVGMarker.h>
#include <CSVG.h>

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
 CSVGObject(svg)
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption        (opt_name, opt_value)) return true;
  if (processStyleOption       (opt_name, opt_value)) return true;
  if (processPresentationOption(opt_name, opt_value)) return true;
  if (processExternalOption    (opt_name, opt_value)) return true;

  std::string     str;
  double          real;
  CSVGLengthValue length;
  CBBox2D         bbox;

  if      (svg_.coordOption (opt_name, opt_value, "refX", &real))
    refX_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "refY", &real))
    refY_ = real;
  else if (svg_.stringOption(opt_name, opt_value, "markerUnits", str))
    markerUnits_ = str;
  else if (svg_.lengthOption(opt_name, opt_value, "markerWidth", length) ||
           svg_.lengthOption(opt_name, opt_value, "marker-width", length))
    markerWidth_ = length.value();
  else if (svg_.lengthOption(opt_name, opt_value, "markerHeight", length) ||
           svg_.lengthOption(opt_name, opt_value, "marker-height", length))
    markerHeight_ = length.value();
  else if (svg_.stringOption(opt_name, opt_value, "orient", str))
    orient_ = str;
  else if (svg_.bboxOption  (opt_name, opt_value, "viewBox", &bbox))
    viewBox_ = bbox;
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

  matrix1.setTranslation(x - refX_.getValue(0), y - refY_.getValue(0));

  matrix2.setScale(markerWidth_.getValue(1)/w1, markerHeight_.getValue(1)/h1);

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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<marker";

    printNameValue(os, "id"          , id_          );
    printNameValue(os, "refX"        , refX_        );
    printNameValue(os, "refY"        , refY_        );
    printNameValue(os, "markerWidth" , markerWidth_ );
    printNameValue(os, "markerHeight", markerHeight_);
    printNameValue(os, "orient"      , orient_      );

    if (viewBox_.isSet())
      os << " viewBox=\"" << viewBox_.getXMin() << " " << viewBox_.getYMin() <<
            " " << viewBox_.getXMax() << " " << viewBox_.getYMax() << "\"";

    printTransform(os);

    os << ">" << std::endl;

    for (const auto &o : objects_)
      o->print(os, hier);

    os << "</marker>" << std::endl;
  }
  else
    os << "marker";
}

std::ostream &
operator<<(std::ostream &os, const CSVGMarker &marker)
{
  marker.print(os, false);

  return os;
}
