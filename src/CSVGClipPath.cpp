#include <CSVGClipPath.h>
#include <CSVG.h>
#include <CSVGBuffer.h>
#include <CStrParse.h>

CSVGClipPath::
CSVGClipPath(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGClipPath::
CSVGClipPath(const CSVGClipPath &path) :
 CSVGObject(path),
 parts_    (path.parts_)
{
}

CSVGClipPath *
CSVGClipPath::
dup() const
{
  return new CSVGClipPath(*this);
}

bool
CSVGClipPath::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string      str;
  CSVGPathPartList parts;
  CSVGCoordUnits   units;

  if      (svg_.pathOption  (opt_name, opt_value, "d", parts))
    parts_ = parts;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "clipPathUnits", units))
    clipPathUnits_ = units;
  else if (svg_.stringOption(opt_name, opt_value, "marker-end", str))
    markerEnd_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "x", str))
    x_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "y", str))
    y_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "width", str))
    width_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "height", str))
    height_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGClipPath::
drawPath(CSVGObject* obj)
{
  CBBox2D bbox;

  if (! obj->getBBox(bbox))
    return;

  //---

  CSVGBuffer *oldBuffer = svg_.getCurrentBuffer();

  // set temp buffer for clip path data
  CSVGBuffer *buffer = svg_.pushBuffer("clipPath_" + obj->getUniqueName());

  buffer->setClip(true);

  //---

  double x = bbox.getXMin  ();
  double y = bbox.getYMin  ();
  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  svg_.beginDrawBuffer(buffer, bbox);

  buffer->clear();

  //---

  CMatrixStack2D transform = buffer->transform();

  // set transform
  CMatrixStack2D transform1;

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CMatrixStack2D m;

    m.translate(x, y);
    m.scale    (w, h);

    transform1 = m;

    svg_.setTransform(m);
  }
  else
    transform1 = transform;

  //---

  // draw clip path
  buffer->pathInit();

  drawSubObject();

  //---

  // take path from buffer and set as clip in oldBuffer
  if (clip_.getRule() == FILL_TYPE_EVEN_ODD)
    oldBuffer->pathEoClip(buffer);
  else
    oldBuffer->pathClip(buffer);

  //---

  // reset transform and switch to old buffer
  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX)
    svg_.setTransform(transform);

  svg_.endDrawBuffer(buffer);

  svg_.popBuffer();
}

void
CSVGClipPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<clipPath";

    CSVGObject::printValues(os);

    printNameParts(os, "d", parts_);

    printNameCoordUnits(os, "clipPathUnits", clipPathUnits_);

    printNameValue(os, "marker-end", markerEnd_);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</clipPath>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else {
    os << "clipPath (";

    os << ")";
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGClipPath &path)
{
  path.print(os, false);

  return os;
}
