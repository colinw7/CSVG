#include <CSVGMask.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

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
 CSVGObject(svg)
{
}

CSVGMask::
CSVGMask(const CSVGMask &mask) :
 CSVGObject(mask),
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string     str;
  double          real;
  CSVGLengthValue length;
  CSVGCoordUnits  units;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_ = length.value();
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length.value();
  else if (svg_.coordUnitsOption(opt_name, opt_value, "maskContentUnits", units))
    units_ = units;
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
  std::string old_buffer = svg_.getBufferName();

  svg_.setBuffer("_mask");

  svg_.getBuffer()->reset();

  CMatrix2D transform;

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CBBox2D bbox;

    if (! object.getBBox(bbox))
      return;

    double x = bbox.getMin().x;
    double y = bbox.getMin().y;
    double w = bbox.getWidth();
    double h = bbox.getHeight();

    CMatrix2D matrix1, matrix2;

    matrix1.setTranslation(x - x_.getValue(0), y - y_.getValue(0));
    matrix2.setScale      (w/width_.getValue(1), h/height_.getValue(1));

    svg_.getTransform(transform);

    svg_.setTransform(transform*matrix1*matrix2);
  }

  drawObject();

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX)
    svg_.setTransform(transform);

  CImagePtr mask_image  = svg_.getBufferImage("_mask");
  CImagePtr mask_image1 = mask_image->createRGBAMask();
  CImagePtr dest_image  = svg_.getBufferImage(old_buffer);

  dest_image->copyAlpha(mask_image1, 0, 0);

  svg_.setBuffer(old_buffer);
}

void
CSVGMask::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<mask";

    printNameValue(os, "id"    , id_    );
    printNameValue(os, "x"     , x_     );
    printNameValue(os, "y"     , y_     );
    printNameValue(os, "width" , width_ );
    printNameValue(os, "height", height_);

    if (getUnitsValid())
      os << " maskContentUnits=\"" << CSVG::encodeUnitsString(getUnits()) << "\"";

    os << ">" << std::endl;

    for (const auto &o : objects_)
      o->print(os, hier);

    os << "</mask>" << std::endl;
  }
  else
    os << "mask ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGMask &mask)
{
  mask.print(os, false);

  return os;
}
