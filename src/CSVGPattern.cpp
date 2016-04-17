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
 CSVGObject(svg)
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
 patternTransform_(pattern.patternTransform_)
{
}

CSVGPattern *
CSVGPattern::
dup() const
{
  return new CSVGPattern(*this);
}

bool
CSVGPattern::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string     str;
  double          real;
  CSVGCoordUnits  units;
  CSVGLengthValue length;
  CMatrix2D       transform;

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
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str)) {
    // TODO: delayed paint ?
    CSVGObject *object;
    CImagePtr   image;

    if (! decodeXLink(opt_value, &object, image))
      return false;

    if  (object) {
      CSVGPattern *p = dynamic_cast<CSVGPattern *>(object);

      if (p) {
        x_                = p->x_;
        y_                = p->y_;
        width_            = p->width_;
        height_           = p->height_;
        units_            = p->units_;
        contentUnits_     = p->contentUnits_;
        patternTransform_ = p->patternTransform_;

        for (const auto &c : children()) {
          CSVGObject *child = c->dup();

          addChildObject(child);
        }
      }
    }
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGPattern::
draw()
{
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

CImagePtr
CSVGPattern::
getImage(CSVGObject *parent, double *w1, double *h1)
{
  double w = 100;
  double h = 100;

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CBBox2D parentBBox;

    if (parent && parent->getBBox(parentBBox)) {
      w = parentBBox.getWidth ();
      h = parentBBox.getHeight();
    }

    *w1 = getWidth ()*w;
    *h1 = getHeight()*h;
  }
  else {
    *w1 = getWidth ();
    *h1 = getHeight();
  }

  //---

  CSVGBuffer *oldBuffer = svg_.getBuffer();

  CMatrix2D transform;

  svg_.getTransform(transform);

  //---

  // switch to pattern buffer
  CSVGBuffer *patternBuffer;

  if (parent)
    patternBuffer = svg_.getBuffer("_" + parent->getUniqueName() + "_pattern");
  else
    patternBuffer = svg_.getBuffer("_pattern");

  svg_.setBuffer(patternBuffer);

  patternBuffer->clear();

  // draw pattern to buffer
  svg_.beginDrawBuffer(patternBuffer, CBBox2D(getX(), getY(), *w1, *h1));

  //---

  if      (getContentsUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    if (parent) {
      CMatrix2D matrix1, matrix2;

      matrix1.setTranslation(0, 0);
      matrix2.setScale      (w, h);

      svg_.setTransform(matrix1*matrix2);
    }
  }
  else if (viewBox_.isValid()) {
    CBBox2D viewBox = viewBox_.getValue();

    CMatrix2D matrix1, matrix2;

    matrix1.setTranslation(0, 0);
    matrix2.setScale      (*w1/viewBox.getWidth(), *h1/viewBox.getHeight());

    svg_.setTransform(matrix1*matrix2);
  }

  drawSubObject();

  //---

  svg_.endDrawBuffer(patternBuffer);

  CImagePtr image = patternBuffer->getImage();

  // restore original buffer
  svg_.setBuffer(oldBuffer);

  if (getContentsUnits() == CSVGCoordUnits::OBJECT_BBOX)
    svg_.setTransform(transform);

  return image;
}

std::ostream &
operator<<(std::ostream &os, const CSVGPattern &pattern)
{
  pattern.print(os, false);

  return os;
}
