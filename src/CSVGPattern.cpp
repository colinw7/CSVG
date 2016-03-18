#include <CSVGI.h>

CSVGPattern::
CSVGPattern(CSVG &svg) :
 CSVGObject    (svg),
 object_       (0),
 x_            (0),
 y_            (0),
 width_        (1),
 height_       (1),
 units_        (CSVG_COORD_UNITS_USER_SPACE),
 content_units_(CSVG_COORD_UNITS_USER_SPACE)
{
}

CSVGPattern::
CSVGPattern(const CSVGPattern &pattern) :
 CSVGObject    (pattern),
 object_       (0),
 x_            (pattern.x_),
 y_            (pattern.y_),
 width_        (pattern.width_),
 height_       (pattern.height_),
 units_        (pattern.units_),
 content_units_(pattern.content_units_)
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
  std::string str;
  double      real;

  if      (svg_.realOption(opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.realOption(opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.stringOption(opt_name, opt_value, "width", str))
    svg_.decodePercentString(str, &width_);
  else if (svg_.stringOption(opt_name, opt_value, "height", str))
    svg_.decodePercentString(str, &height_);
  else if (svg_.stringOption(opt_name, opt_value, "patternUnits", str))
    svg_.decodeUnitsString(str, &units_);
  else if (svg_.stringOption(opt_name, opt_value, "patternContentUnits", str))
    svg_.decodeUnitsString(str, &content_units_);
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str)) {
    CSVGObject *object;
    CImagePtr   image;

    if (! decodeXLink(opt_value, &object, image))
      return false;

    if  (object != 0) {
      CSVGPattern *p = dynamic_cast<CSVGPattern *>(object);

      if (p != 0) {
        x_             = p->x_;
        y_             = p->y_;
        width_         = p->width_;
        height_        = p->height_;
        units_         = p->units_;
        content_units_ = p->content_units_;

        ObjectList::const_iterator po1, po2;

        for (po1 = p->objects_.begin(), po2 = p->objects_.end(); po1 != po2; ++po1) {
          CSVGObject *child = (*po1)->dup();

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
print(std::ostream &os) const
{
  os << "pattern ";
}

CImagePtr
CSVGPattern::
getImage(double w, double h, double *w1, double *h1)
{
  CBBox2D bbox;

  if (! getBBox(bbox))
    return CImagePtr();

  if (units_ == CSVG_COORD_UNITS_OBJECT_BBOX) {
    *w1 = width_ *w;
    *h1 = height_*h;
  }
  else {
    *w1 = width_;
    *h1 = height_;
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
  pattern.print(os);

  return os;
}
