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

  if      (svg_.stringOption(opt_name, opt_value, "d", str)) {
    if (! svg_.pathStringToParts(str, parts_))
      return false;
  }
  else if (svg_.stringOption(opt_name, opt_value, "clipPathUnits", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "marker-end", str))
    ;
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
    if (! objects_.empty()) {
      os << "<clipPath";

      std::string id = getId();

      if (id != "")
        os << " id=\"" << id << "\"";

      os << ">" << std::endl;

      for (const auto &o : objects_)
        o->print(os, hier);

      os << "</clipPath>" << std::endl;
    }
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
