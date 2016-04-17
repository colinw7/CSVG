#include <CSVGPath.h>
#include <CSVGPathPart.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>

CSVGPath::
CSVGPath(CSVG &svg) :
 CSVGObject(svg),
 parts_    ()
{
  //fill_.setDefColor(CRGBA(0,0,0));
}

CSVGPath::
CSVGPath(const CSVGPath &path) :
 CSVGObject(path),
 parts_    (path.parts_)
{
}

CSVGPath *
CSVGPath::
dup() const
{
  return new CSVGPath(*this);
}

bool
CSVGPath::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  PartList parts;

  if (svg_.pathOption(opt_name, opt_value, "d", parts))
    parts_ = parts;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGPath::
moveBy(const CVector2D &d)
{
  for (const auto &p : parts_)
    p->moveBy(d);
}

void
CSVGPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  svg_.drawParts(parts_, &marker_);
}

void
CSVGPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<path";

    CSVGObject::printValues(os);

    printNameParts(os, "d", parts_);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</path>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else {
    os << "path (";

    svg_.printParts(os, parts_);

    os << ")";
  }
}

bool
CSVGPath::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid())
    return svg_.getPartsBBox(parts_, bbox);
  else {
    bbox = getViewBox();

    return true;
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGPath &path)
{
  path.print(os, false);

  return os;
}
