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
  std::string     str;
  double          real;
  CSVGCoordUnits  units;
  CSVGLengthValue length;
  CMatrixStack2D  transform;

  if      (svg_.realOption(opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.realOption(opt_name, opt_value, "y", &real))
    y_ = real;
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

    CSVGObject::printValues(os);

    printNameValue(os, "x", x_);
    printNameValue(os, "y", y_);

    printNameLength(os, "width" , width_ );
    printNameLength(os, "height", height_);

    printNameCoordUnits(os, "patternUnits"       , units_);
    printNameCoordUnits(os, "patternContentUnits", contentUnits_);

    printNameTransform(os, "patternTransform", patternTransform_);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</pattern>" << std::endl;
  }
  else
    os << "pattern ";
}

void
CSVGPattern::
setFillImage(CSVGObject *parent, CSVGBuffer *buffer, double *w1, double *h1)
{
  double w = getWidth ();
  double h = getHeight();

  double pw = 100;
  double ph = 100;

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CBBox2D parentBBox;

    if (parent && parent->getBBox(parentBBox)) {
      pw = parentBBox.getWidth ();
      ph = parentBBox.getHeight();
    }

    *w1 = w*pw;
    *h1 = h*ph;
  }
  else {
    *w1 = w;
    *h1 = h;
  }

  //---

  CSVGBuffer *oldBuffer = svg_.getBuffer();

  CMatrixStack2D transform = oldBuffer->transform();

  //---

  // switch to pattern buffer
  CSVGBuffer *patternBuffer;

  std::string bufferId = (getId() != "" ? getId() : "pattern");

  if (parent)
    patternBuffer = svg_.getBuffer("_" + parent->getUniqueName() + "_" + bufferId);
  else
    patternBuffer = svg_.getBuffer("_" + bufferId);

  svg_.setBuffer(patternBuffer);

  patternBuffer->clear();

  // draw pattern to buffer
  svg_.beginDrawBuffer(patternBuffer, CBBox2D(getX(), getY(), *w1, *h1));
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
  else if (viewBox_.isValid()) {
    CBBox2D viewBox = viewBox_.getValue();

    //matrix.translate(0, 0);
    matrix.scale(*w1/viewBox.getWidth(), *h1/viewBox.getHeight());

    svg_.setTransform(matrix);
  }

  //--

  drawSubObject();

  //---

  svg_.endDrawBuffer(patternBuffer);

  // restore original buffer
  svg_.setBuffer(oldBuffer);

  svg_.setTransform(transform);

  //---

  // set fill image from buffer
  buffer->setFillBuffer(patternBuffer);
}

CSVGObject *
CSVGPattern::
getObject()
{
  if (! xlink_.isValid())
    return 0;

  return xlink_.getValue().getObject();
}

std::ostream &
operator<<(std::ostream &os, const CSVGPattern &pattern)
{
  pattern.print(os, false);

  return os;
}
