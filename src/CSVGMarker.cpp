#include <CSVGMarker.h>
#include <CSVGBuffer.h>
#include <CSVGLog.h>
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
 CSVGObject     (marker),
 refX_          (marker.refX_),
 refY_          (marker.refY_),
 markerUnits_   (marker.markerUnits_),
 markerWidth_   (marker.markerWidth_),
 markerHeight_  (marker.markerHeight_),
 orient_        (marker.orient_),
 preserveAspect_(marker.preserveAspect_)
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

  std::string        str;
  double             real;
  CBBox2D            bbox;
  CSVGCoordUnits     units;
  CScreenUnits       length;
  CSVGPreserveAspect preserveAspect;

  if      (svg_.coordOption (opt_name, opt_value, "refX", &real))
    refX_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "refY", &real))
    refY_ = real;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "markerUnits", units))
    markerUnits_ = units;
  else if (svg_.lengthOption(opt_name, opt_value, "markerWidth", length) ||
           svg_.lengthOption(opt_name, opt_value, "marker-width", length))
    markerWidth_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "markerHeight", length) ||
           svg_.lengthOption(opt_name, opt_value, "marker-height", length))
    markerHeight_ = length;
  else if (svg_.stringOption(opt_name, opt_value, "orient", str))
    orient_ = str;
  else if (svg_.bboxOption  (opt_name, opt_value, "viewBox", &bbox))
    viewBox_ = bbox;
  else if (svg_.preserveAspectOption(opt_name, opt_value, "preserveAspectRatio", preserveAspect))
    preserveAspect_ = preserveAspect;
  else
    return false;

  return true;
}

void
CSVGMarker::
drawMarker(double x, double y, double autoAngle)
{
  // draw marker at (x,y) at specified angle (offset by and rotated around refX, refY).
  // marker drawn in box (0, 0, markerWidth, markerHeight) in markerUnits.
  // if angle is auto then autoAngle used, otherwise specified angle used (default 0).

  double angle = 0;

  if (orient_.isValid()) {
    if (orient_.getValue() != "auto") {
      double r;

      if (! CStrUtil::toReal(orient_.getValue(), &r))
        angle = r;
    }
    else
      angle = autoAngle;
  }

  //---

  CSVGBuffer *currentBuffer = svg_.getCurrentBuffer();

  CMatrixStack2D transform = currentBuffer->transform();

  double refX = getRefX();
  double refY = getRefY();

  double markerWidth  = getMarkerWidth ().pxValue();
  double markerHeight = getMarkerHeight().pxValue();

  CMatrixStack2D matrix;

  matrix.append(transform);

  CBBox2D bbox;

  if (hasViewBox())
    bbox = getViewBox();
  else
    getChildrenBBox(bbox);

//double x1 = 0, y1 = 0;
  double w1 = 1, h1 = 1;

  if (bbox.isSet()) {
  //x1 = bbox.getXMin();
  //y1 = bbox.getYMin();
    w1 = bbox.getWidth ();
    h1 = bbox.getHeight();
  }

  double xs = 1, ys = 1;

  if      (hasMarkerUnits()) {
    if      (getMarkerUnits() == CSVGCoordUnits::USER_SPACE) {
    //double xs = markerWidth /w1;
    //double ys = markerHeight/h1;
      xs = w1/markerWidth;
      ys = h1/markerHeight;
    }
    else if (getMarkerUnits() == CSVGCoordUnits::STROKE_WIDTH) {
      double lw = 1;

      if (svg_.currentDrawObject())
        lw = svg_.currentDrawObject()->getFlatStrokeWidth();

      xs = markerWidth*lw/w1;
      ys = markerWidth*lw/h1;
    }
    else {
      CSVGLog() << "Invalid marker units";
    }
  }
  else if (hasViewBox()) {
    // fit inside view box
    CBBox2D viewBox = getViewBox();

    w1 = viewBox.getWidth ();
    h1 = viewBox.getHeight();

    xs = w1/markerWidth;
    ys = h1/markerHeight;
  }
  else {
  //double xs = markerWidth /w1;
  //double ys = markerHeight/h1;
    xs = w1/markerWidth;
    ys = h1/markerHeight;
  }

  matrix.translate(x - refX*xs, y - refY*ys);

  matrix.rotate(angle, CPoint2D(refX*xs, refY*ys));

  matrix.scale(xs, ys);

  currentBuffer->setTransform(matrix);

  //---

  bool clip = true;

  if (hasOverflow()) {
    if (getOverflow() == CSVGOverflowType::VISIBLE || getOverflow() == CSVGOverflowType::AUTO)
      clip = false;
  }

  if (clip) {
    // draw rectangle as clip
    CSVGBuffer *buffer = svg_.pushBuffer("clipPath_" + getUniqueName());

    buffer->setClip(true);

    svg_.beginDrawBuffer(buffer, bbox);

    buffer->clear();

    //buffer->setTransform(matrix);

    buffer->pathInit();

    // expand for line width/alias
    CBBox2D bbox1 = bbox.expanded(-1, -1, 1, 1);

    buffer->drawRectangle(bbox1);

    currentBuffer->pathClip(buffer);

    svg_.endDrawBuffer(buffer);

    svg_.popBuffer();
  }

  //---

  // draw children
  for (const auto &c : children())
    c->drawObject();

  //---

  if (clip)
    currentBuffer->initClip();

  //---

  currentBuffer->setTransform(transform);
}

void
CSVGMarker::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<marker";

    CSVGObject::printValues(os);

    printNameValue(os, "refX", refX_);
    printNameValue(os, "refY", refY_);

    printNameCoordUnits(os, "markerUnits", markerUnits_);

    printNameLength(os, "markerWidth" , markerWidth_ );
    printNameLength(os, "markerHeight", markerHeight_);

    printNameValue(os, "orient", orient_);

    printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);

    os << ">" << std::endl;

    printChildren(os, hier);

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
