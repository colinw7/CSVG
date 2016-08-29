#include <CSVGPattern.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

/* Attributes:
    <Core>
    <Conditional>
    <Presentational>
    <XLink>
    class
    style
    externalResourcesRequired
    viewBox
    preserveAspectRatio
    x
    y
    width
    height
    patternUnits
    patternContentUnits
    patternTransform
    xlink:href
*/
CSVGPattern::
CSVGPattern(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGPattern::
CSVGPattern(const CSVGPattern &pattern) :
 CSVGObject(pattern),
 x_               (pattern.x_),
 y_               (pattern.y_),
 width_           (pattern.width_),
 height_          (pattern.height_),
 units_           (pattern.units_),
 contentUnits_    (pattern.contentUnits_),
 patternTransform_(pattern.patternTransform_),
 xlink_           (pattern.xlink_)
{
}

CSVGPattern *
CSVGPattern::
dup() const
{
  return new CSVGPattern(*this);
}

void
CSVGPattern::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

bool
CSVGPattern::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string    str;
  CSVGCoordUnits units;
  CScreenUnits   length;
  CMatrixStack2D transform;

  if      (svg_.coordOption(opt_name, opt_value, "x", length))
    x_ = length;
  else if (svg_.coordOption(opt_name, opt_value, "y", length))
    y_ = length;
  else if (svg_.percentOption(opt_name, opt_value, "width", length))
    width_ = length;
  else if (svg_.percentOption(opt_name, opt_value, "height", length))
    height_ = length;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "patternUnits", units))
    units_ = units;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "patternContentUnits", units))
    contentUnits_ = units;
  else if (svg_.transformOption(opt_name, opt_value, "patternTransform", transform))
    patternTransform_ = transform;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGPattern::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<pattern";

    printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</pattern>" << std::endl;
  }
  else
    os << "pattern ";
}

void
CSVGPattern::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue(os, "x", x_);
  printNameValue(os, "y", y_);

  printNamePercent(os, "width" , width_ );
  printNamePercent(os, "height", height_);

  if (! xlink_.getValue().isNull())
    printNameXLink(os, "xlink:href", xlink_);

  printNameCoordUnits(os, "patternUnits"       , units_);
  printNameCoordUnits(os, "patternContentUnits", contentUnits_);

  printNameTransform(os, "patternTransform", patternTransform_);
}

void
CSVGPattern::
setFillImage(CSVGObject *parent, CSVGBuffer *buffer,
             double *x1, double *y1, double *w1, double *h1)
{
  CSVGPattern *linkPattern = dynamic_cast< CSVGPattern *>(getObject());

  if (linkPattern) {
    if (patternTransform_.isValid())
      buffer->setFillMatrix(patternTransform_.getValue().getMatrix());

    return linkPattern->setFillImage(parent, buffer, x1, y1, w1, h1);
  }

  //---

  double pw = 100;
  double ph = 100;

  CBBox2D parentBBox;

  if (parent && parent->getBBox(parentBBox)) {
    pw = parentBBox.getWidth ();
    ph = parentBBox.getHeight();
  }

  double x = getX     ().pxValue(1);
  double y = getY     ().pxValue(1);
  double w = getWidth ().pxValue(1);
  double h = getHeight().pxValue(1);

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    *x1 = x*pw;
    *y1 = y*ph;
    *w1 = w*pw;
    *h1 = h*ph;
  }
  else {
    *x1 = x;
    *y1 = y;
    *w1 = w;
    *h1 = h;
  }

  //---

  CSVGBuffer *oldBuffer = svg_.getCurrentBuffer();

  CMatrixStack2D transform = oldBuffer->transform();

  //---

  // switch to pattern buffer
  CSVGBuffer *patternBuffer;

  std::string bufferId = (getId() != "" ? getId() : "pattern");

  if (parent)
    patternBuffer = svg_.pushBuffer("_" + parent->getUniqueName() + "_" + bufferId);
  else
    patternBuffer = svg_.pushBuffer("_" + bufferId);

  patternBuffer->clear();

  // draw pattern to buffer
  CBBox2D patternBBox(0, 0, pw, ph);

  svg_.beginDrawBuffer(patternBuffer, patternBBox);

  //---

  CMatrixStack2D matrix;

  if      (hasViewBox()) {
    CBBox2D viewBox = viewBox_.getValue();

    //matrix.translate(0, 0);
    matrix.scale(*w1/viewBox.getWidth(), *h1/viewBox.getHeight());

    svg_.setTransform(matrix);
  }
  else if (getContentUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    if (parent) {
      //matrix.translate(0, 0);
      matrix.scale(pw, ph);

      svg_.setTransform(matrix);
    }
  }

  //--

  svg_.pushStyle(nullptr);

  if (getDisplay() != "none")
    drawSubObject();

  svg_.popStyle();

  //---

  svg_.endDrawBuffer(patternBuffer);

  // restore original buffer
  svg_.popBuffer();

  svg_.setTransform(transform);

  //---

  // set fill image from buffer

  // TODO *x1, *y1 for brush origin
  CBBox2D patternBBox1(0, 0, *w1, *h1);

  patternBuffer->setBBox(patternBBox1);

  buffer->setFillBuffer(*x1, *y1, patternBuffer);

  if (patternTransform_.isValid())
    buffer->setFillMatrix(patternTransform_.getValue().getMatrix());
}

void
CSVGPattern::
setStrokeImage(CSVGObject *parent, CSVGBuffer *buffer, double *w1, double *h1)
{
  double pw = 100;
  double ph = 100;

  CBBox2D parentBBox;

  if (parent && parent->getBBox(parentBBox)) {
    pw = parentBBox.getWidth ();
    ph = parentBBox.getHeight();
  }

  double w = getWidth ().pxValue(1);
  double h = getHeight().pxValue(1);

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    *w1 = w*pw;
    *h1 = h*ph;
  }
  else {
    *w1 = w;
    *h1 = h;
  }

  //---

  CSVGBuffer *oldBuffer = svg_.getCurrentBuffer();

  CMatrixStack2D transform = oldBuffer->transform();

  //---

  // switch to pattern buffer
  CSVGBuffer *patternBuffer;

  std::string bufferId = (getId() != "" ? getId() : "pattern");

  if (parent)
    patternBuffer = svg_.pushBuffer("_" + parent->getUniqueName() + "_" + bufferId);
  else
    patternBuffer = svg_.pushBuffer("_" + bufferId);

  patternBuffer->clear();

  // draw pattern to buffer
  double x = getX().pxValue(pw);
  double y = getY().pxValue(ph);

  svg_.beginDrawBuffer(patternBuffer, CBBox2D(x, y, *w1, *h1));
  //svg_.beginDrawBuffer(patternBuffer);

  //---

  CMatrixStack2D matrix;

  if      (getContentUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    if (parent) {
      //matrix.translate(0, 0);
      matrix.scale(*w1/w, *h1/h);

      svg_.setTransform(matrix);
    }
  }
  else if (hasViewBox()) {
    CBBox2D viewBox = viewBox_.getValue();

    //matrix.translate(0, 0);
    matrix.scale(*w1/viewBox.getWidth(), *h1/viewBox.getHeight());

    svg_.setTransform(matrix);
  }

  //--

  if (getDisplay() != "none")
    drawSubObject();

  //---

  svg_.endDrawBuffer(patternBuffer);

  // restore original buffer
  svg_.popBuffer();

  svg_.setTransform(transform);

  //---

  // set fill image from buffer
  buffer->setStrokeFillBuffer(patternBuffer);
}

CSVGObject *
CSVGPattern::
getObject()
{
  if (! xlink_.isValid())
    return nullptr;

  return xlink_.getValue().getObject();
}

std::ostream &
operator<<(std::ostream &os, const CSVGPattern &pattern)
{
  pattern.print(os, false);

  return os;
}
