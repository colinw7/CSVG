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
  std::string    str;
  PartList       parts;
  CSVGCoordUnits units;

  if      (svg_.pathOption  (opt_name, opt_value, "d", parts))
    parts_ = parts;
  else if (svg_.coordUnitsOption(opt_name, opt_value, "clipPathUnits", units))
    clipPathUnits_ = units;
  else if (svg_.stringOption(opt_name, opt_value, "marker-end", str))
    markerEnd_ = str;
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

  CSVGBuffer *oldBuffer = svg_.getBuffer();

  // set temp buffer for clip path data
  CSVGBuffer *buffer = svg_.getBuffer("clipPath_" + obj->getUniqueName());

  svg_.setBuffer(buffer);

  //---

  double x = bbox.getXMin  ();
  double y = bbox.getYMin  ();
  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  svg_.beginDrawBuffer(buffer, bbox);

  buffer->clear();

  //---

  // draw clip path
  CMatrix2D transform;

  svg_.getTransform(transform);

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX) {
    CMatrix2D matrix1, matrix2;

    matrix1.setTranslation(x, y);
    matrix2.setScale      (w, h);

    svg_.setTransform(matrix1*matrix2);
  }

  svg_.pathInit();

  drawSubObject();

  if (getUnits() == CSVGCoordUnits::OBJECT_BBOX)
    svg_.setTransform(transform);

  //---

  svg_.endDrawBuffer(buffer);

  svg_.setBuffer(oldBuffer);

  //---

  if (clip_.getRule() == FILL_TYPE_EVEN_ODD)
    oldBuffer->pathEoClip(buffer);
  else
    oldBuffer->pathClip(buffer);
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
