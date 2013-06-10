#include <CSVGI.h>
#include <CStrParse.h>

CSVGClipPath::
CSVGClipPath(CSVG &svg) :
 CSVGObject(svg), parts_()
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
processOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "d", str)) {
    if (! svg_.pathStringToParts(str, parts_))
      return false;
  }
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
print(ostream &os) const
{
  os << "clipPath (";

  os << ")";
}

ostream &
operator<<(ostream &os, const CSVGClipPath &path)
{
  path.print(os);

  return os;
}
