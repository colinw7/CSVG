#include <CSVGClipPath.h>
#include <CSVG.h>
#include <CSVGLog.h>
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
  std::string str;
  PartList    parts;

  if      (svg_.pathOption  (opt_name, opt_value, "d", parts))
    parts_ = parts;
  else if (svg_.stringOption(opt_name, opt_value, "clipPathUnits", str))
    clipPathUnits_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "marker-end", str))
    markerEnd_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGClipPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;
}

void
CSVGClipPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<clipPath";

    CSVGObject::printValues(os);

    printNameParts(os, "d"            , parts_        );
    printNameValue(os, "clipPathUnits", clipPathUnits_);
    printNameValue(os, "marker-end"   , markerEnd_    );

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
