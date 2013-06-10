#include "CSVGI.h"

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Presentation>
    <External>
    x <Coordinate>
    y <Coordinate>
    width <Length>
    height <Length>
    maskUnits ( userSpaceOnUse | objectBoundingBox )
    maskContentUnits ( userSpaceOnUse | objectBoundingBox )
*/

CSVGMask::
CSVGMask(CSVG &svg) :
 CSVGObject(svg),
 object_   (NULL),
 x_        (0),
 y_        (0),
 width_    (1),
 height_   (1),
 units_    (CSVG_COORD_UNITS_USER_SPACE)
{
}

CSVGMask::
CSVGMask(const CSVGMask &mask) :
 CSVGObject(mask),
 object_   (NULL),
 x_        (mask.x_),
 y_        (mask.y_),
 width_    (mask.width_),
 height_   (mask.height_),
 units_    (mask.units_)
{
}

CSVGMask *
CSVGMask::
dup() const
{
  return new CSVGMask(*this);
}

bool
CSVGMask::
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", &real))
    width_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "height", &real))
    height_ = real;
  else if (svg_.stringOption(opt_name, opt_value, "maskContentUnits", str))
    svg_.decodeUnitsString(str, &units_);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGMask::
draw()
{
}

void
CSVGMask::
objectDraw(const CSVGObject &object)
{
  string old_buffer = svg_.getBufferName();

  svg_.setBuffer("_mask");

  svg_.getBuffer()->reset();

  CMatrix2D transform;

  if (units_ == CSVG_COORD_UNITS_OBJECT_BBOX) {
    CBBox2D bbox;

    if (! object.getBBox(bbox))
      return;

    double x = bbox.getMin().x;
    double y = bbox.getMin().y;
    double w = bbox.getWidth();
    double h = bbox.getHeight();

    CMatrix2D matrix1, matrix2;

    matrix1.setTranslation(x - x_, y - y_);
    matrix2.setScale      (w/width_, h/height_);

    svg_.getTransform(transform);

    svg_.setTransform(transform*matrix1*matrix2);
  }

  drawObject();

  if (units_ == CSVG_COORD_UNITS_OBJECT_BBOX)
    svg_.setTransform(transform);

  CImagePtr mask_image  = svg_.getBufferImage("_mask");
  CImagePtr mask_image1 = mask_image->createRGBAMask();
  CImagePtr dest_image  = svg_.getBufferImage(old_buffer);

  dest_image->combineAlpha(mask_image1, 0, 0);

  svg_.setBuffer(old_buffer);
}

void
CSVGMask::
print(ostream &os) const
{
  os << "mask ";
}

ostream &
operator<<(ostream &os, const CSVGMask &mask)
{
  mask.print(os);

  return os;
}
