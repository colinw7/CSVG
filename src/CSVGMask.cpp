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
  CSVGCoordUnits  units;
  CSVGLengthValue length;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length;
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
drawMask(const CSVGObject &object)
{
  CBBox2D bbox;

  if (! object.getBBox(bbox))
    return;

  //---

  CSVGBuffer *oldBuffer = svg_.getBuffer();

  // set temp buffer for mask image
  CSVGBuffer *buffer = svg_.getBuffer("mask_" + object.getUniqueName());

  svg_.setBuffer(buffer);

  //---

  double x = bbox.getXMin  ();
  double y = bbox.getYMin  ();
  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  svg_.beginDrawBuffer(buffer, bbox);

  buffer->clear();

  //---

  // draw mask object
  CMatrixStack2D transform;

  svg_.getTransform(transform);

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CMatrixStack2D matrix;

    matrix.translate(x - getX()  , y - getY()   );
    matrix.scale    (w/getWidth(), h/getHeight());

    svg_.setTransform(matrix);
  }

  drawSubObject();

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX)
    svg_.setTransform(transform);

  //---

  svg_.endDrawBuffer(buffer);

  svg_.setBuffer(oldBuffer);

  //---

  // create mask image
  CImagePtr mask_image  = buffer->getImage();
  CImagePtr mask_image1 = mask_image->createRGBAMask();

#if 0
  {
  std::string maskBufferName = "rgb_mask_" + object.getUniqueName();

  CSVGBuffer *buffer = svg_.getBuffer(maskBufferName);

  buffer->setImage(mask_image1);
  }
#endif

  //---

  // combine mask with image
  CImagePtr dest_image = oldBuffer->getImage();

  dest_image->copyAlpha(mask_image1, 0, 0);


#if 0
  {
  std::string maskBufferName = "alpha_mask_" + object.getUniqueName();

  CSVGBuffer *buffer = svg_.getBuffer(maskBufferName);

  buffer->setImage(dest_image);
  }
#endif

  oldBuffer->setImage(dest_image);
}

void
CSVGMask::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<mask";

    CSVGObject::printValues(os);

    printNameValue (os, "x"     , x_     );
    printNameValue (os, "y"     , y_     );
    printNameLength(os, "width" , width_ );
    printNameLength(os, "height", height_);

    if (getUnitsValid())
      os << " maskContentUnits=\"" << CSVG::encodeUnitsString(getUnits()) << "\"";

    os << ">" << std::endl;

    printChildren(os, hier);

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
