#include <CSVGMPath.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>

CSVGMPath::
CSVGMPath(CSVG &svg) :
 CSVGObject(svg),
 parts_    (),
 xlink_    (this)
{
}

CSVGMPath::
CSVGMPath(const CSVGMPath &path) :
 CSVGObject(path),
 parts_    (path.parts_),
 xlink_    (this)
{
}

CSVGMPath *
CSVGMPath::
dup() const
{
  return new CSVGMPath(*this);
}

void
CSVGMPath::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

bool
CSVGMPath::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string      str;
  CSVGPathPartList parts;

  if      (svg_.pathOption  (opt_name, opt_value, "d", parts))
    parts_ = parts;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGMPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  return false;
}

void
CSVGMPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<mpath";

    CSVGObject::printValues(os);

    printNameXLink(os, "xlink:href", xlink_);

    printNameParts(os, "d", parts_);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</mpath>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else {
    os << "mpath (" << parts_ << ")";
  }
}

bool
CSVGMPath::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox()) {
    CSVGBuffer *currentBuffer = svg_.getCurrentBuffer();

    return parts_.getBBox(currentBuffer, bbox);
  }
  else {
    bbox = getViewBox();

    return true;
  }
}

std::ostream &
operator<<(std::ostream &os, const CSVGMPath &path)
{
  path.print(os, false);

  return os;
}
