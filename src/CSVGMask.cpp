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
 x_           (mask.x_),
 y_           (mask.y_),
 width_       (mask.width_),
 height_      (mask.height_),
 units_       (mask.units_),
 contentUnits_(mask.contentUnits_)
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
  std::string    str;
  double         real;
  CSVGCoordUnits units;
  CScreenUnits   length;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_ = length;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "maskUnits", units))
    units_ = units;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "maskContentUnits", units))
    contentUnits_ = units;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGMask::
drawMask(const CSVGObject *object)
{
  CBBox2D bbox;

  if (! object->getBBox(bbox))
    return;

  //---

  auto *oldBuffer = svg_.getCurrentBuffer();

  // set temp buffer for mask image
  auto *buffer = svg_.pushBuffer("mask_" + object->getUniqueName());

  double x = bbox.getXMin  ();
  double y = bbox.getYMin  ();
  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  svg_.beginDrawBuffer(buffer, bbox);

  buffer->clear();

  //---

  auto transform = buffer->transform();

  // set transform
  CMatrixStack2D transform1;

  if (getContentUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CMatrixStack2D matrix;

    matrix.addTranslation(x - getX()  , y - getY()   );
    matrix.addScale      (w/getWidth(), h/getHeight());

    transform1 = matrix;

    svg_.setTransform(matrix);
  }
  else
    transform1 = transform;

  // draw mask children
  (void) drawSubObject();

  // reset transform
  if (getContentUnits() == CSVGCoordUnits::OBJECT_BBOX)
    svg_.setTransform(transform);

  //---

  svg_.endDrawBuffer(buffer);

  svg_.popBuffer();

  //---

  // get offset
  double x1 = 0, y1 = 0;

  if (getContentUnits() == CSVGCoordUnits::OBJECT_BBOX)
    transform1.multiplyPoint(0, 0, &x1, &y1);
  else
    transform1.multiplyPoint(x, y, &x1, &y1);

  double px1, py1;

  svg_.lengthToPixel(x1, y1, &px1, &py1);

  //---

  CSVGBuffer::maskBuffers(oldBuffer, buffer, object, px1, py1);
}

void
CSVGMask::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<mask";

    printValues(os);

    os << ">\n";

    printChildren(os, hier);

    os << "</mask>\n";
  }
  else
    os << "mask ";
}

void
CSVGMask::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue (os, "x"     , x_     );
  printNameValue (os, "y"     , y_     );
  printNameLength(os, "width" , width_ );
  printNameLength(os, "height", height_);

  if (getUnitsValid())
    os << " maskUnits=\"" << CSVG::encodeUnitsString(getUnits()) << "\"";

  if (getContentUnitsValid())
    os << " maskContentUnits=\"" << CSVG::encodeUnitsString(getContentUnits()) << "\"";
}
