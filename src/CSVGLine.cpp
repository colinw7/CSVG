#include <CSVGLine.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGLine::
CSVGLine(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGLine::
CSVGLine(const CSVGLine &line) :
 CSVGObject(line),
 x1_       (line.x1_),
 y1_       (line.y1_),
 x2_       (line.x2_),
 y2_       (line.y2_)
{
}

CSVGLine *
CSVGLine::
dup() const
{
  return new CSVGLine(*this);
}

bool
CSVGLine::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processPaintOption          (opt_name, opt_value)) return true;
  if (processColorOption          (opt_name, opt_value)) return true;
  if (processOpacityOption        (opt_name, opt_value)) return true;
  if (processGraphicsOption       (opt_name, opt_value)) return true;
  if (processMarkerOption         (opt_name, opt_value)) return true;
  if (processClipOption           (opt_name, opt_value)) return true;
  if (processMaskOption           (opt_name, opt_value)) return true;
  if (processFilterOption         (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processCursorOption         (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  std::string     str;
  CSVGLengthValue length;

  if      (svg_.coordOption(opt_name, opt_value, "x1", length))
    x1_ = length;
  else if (svg_.coordOption(opt_name, opt_value, "y1", length))
    y1_ = length;
  else if (svg_.coordOption(opt_name, opt_value, "x2", length))
    x2_ = length;
  else if (svg_.coordOption(opt_name, opt_value, "y2", length))
    y2_ = length;
  else if (svg_.stringOption(opt_name, opt_value, "transform", str)) {
    CMatrix2D transform;

    if (! svg_.decodeTransform(str, transform))
      return false;

    setTransform(transform);
  }
  else
    return false;

  return true;
}

void
CSVGLine::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.pathInit();

  svg_.pathMoveTo(getX1(), getY1());
  svg_.pathLineTo(getX2(), getY2());

  if (svg_.isFilled())
    svg_.pathFill();

  if (svg_.isStroked())
    svg_.pathStroke();
}

bool
CSVGLine::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isSet())
    bbox = CBBox2D(getStart(), getEnd());
  else
    bbox = viewBox_;

  return true;
}

void
CSVGLine::
moveBy(const CVector2D &delta)
{
  setStart(getStart() + delta);
  setEnd  (getEnd  () + delta);
}

void
CSVGLine::
resizeTo(const CSize2D &size)
{
  setX2(getX1() + size.width );
  setY2(getY1() + size.height);
}

void
CSVGLine::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<line";

    printNameValue(os, "id", id_);

    printNameLength(os, "x1", x1_);
    printNameLength(os, "y1", y1_);
    printNameLength(os, "x2", x2_);
    printNameLength(os, "y2", y2_);

    printStyle(os);

    os << "/>" << std::endl;
  }
  else
    os << "line " << getStart() << " " << getEnd();
}

std::ostream &
operator<<(std::ostream &os, const CSVGLine &line)
{
  line.print(os, false);

  return os;
}
