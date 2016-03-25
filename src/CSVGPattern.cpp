#include <CSVGPattern.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGPattern::
CSVGPattern(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGPattern::
CSVGPattern(const CSVGPattern &pattern) :
 CSVGObject   (pattern),
 object_      (0),
 x_           (pattern.x_),
 y_           (pattern.y_),
 width_       (pattern.width_),
 height_      (pattern.height_),
 units_       (pattern.units_),
 contentUnits_(pattern.contentUnits_)
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

  if      (svg_.realOption(opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.realOption(opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.percentOption(opt_name, opt_value, "width", length))
    width_ = length.value();
  else if (svg_.percentOption(opt_name, opt_value, "height", length))
    height_ = length.value();
  else if (svg_.coordUnitsOption(opt_name, opt_value, "patternUnits", units))
    units_ = units;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "patternContentUnits", units))
    contentUnits_ = units;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str)) {
    CSVGObject *object;
    CImagePtr   image;

    if (! decodeXLink(opt_value, &object, image))
      return false;

    if  (object != 0) {
      CSVGPattern *p = dynamic_cast<CSVGPattern *>(object);

      if (p != 0) {
        x_            = p->x_;
        y_            = p->y_;
        width_        = p->width_;
        height_       = p->height_;
        units_        = p->units_;
        contentUnits_ = p->contentUnits_;

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

    printNameValue(os, "id", id_);
    printNameValue(os, "x" , x_ );
    printNameValue(os, "y" , y_ );

    if (width_.isValid())
      os << " width=\"" << 100*width_.getValue() << "%\"";

    if (height_.isValid())
      os << " height=\"" << 100*height_.getValue() << "%\"";

    if (getUnitsValid())
      os << " patternUnits=\"" << CSVG::encodeUnitsString(getUnits()) << "\"";

    if (getContentsUnitsValid())
      os << " patternContentUnits=\"" << CSVG::encodeUnitsString(getContentsUnits()) << "\"";

    printTransform(os);

    os << ">" << std::endl;

    for (const auto &o : objects_)
      o->print(os, hier);

    os << "</pattern>" << std::endl;
  }
  else
    os << "pattern ";
}

CImagePtr
CSVGPattern::
getImage(double w, double h, double *w1, double *h1)
{
  CBBox2D bbox;

  if (! getBBox(bbox))
    return CImagePtr();

  if (units_ == CSVGCoordUnits::OBJECT_BBOX) {
    *w1 = width_ .getValue(1)*w;
    *h1 = height_.getValue(1)*h;
  }
  else {
    *w1 = width_ .getValue(1);
    *h1 = height_.getValue(1);
  }

  std::string old_buffer = svg_.getBufferName();

  svg_.setBuffer("_pattern");

  CSVGBuffer *buffer = svg_.getBuffer();

  buffer->setup(bbox);

  drawObject();

  CImagePtr image = svg_.getBufferImage("_pattern");

  svg_.setBuffer(old_buffer);

  return image;
}

std::ostream &
operator<<(std::ostream &os, const CSVGPattern &pattern)
{
  pattern.print(os, false);

  return os;
}
