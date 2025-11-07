#include <CSVGColorProfile.h>
#include <CSVG.h>

CSVGColorProfile::
CSVGColorProfile(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGColorProfile::
CSVGColorProfile(const CSVGColorProfile &c) :
 CSVGObject(c),
 local_    (c.local_),
 name_     (c.name_),
 intent_   (c.intent_),
 xlink_    (c.xlink_)
{
}

CSVGColorProfile *
CSVGColorProfile::
dup() const
{
  return new CSVGColorProfile(*this);
}

bool
CSVGColorProfile::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "local", str))
    local_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "name", str))
    name_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "rendering-intent", str))
    intent_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGColorProfile::
draw()
{
  return false;
}

void
CSVGColorProfile::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<color-profile";

    printValues(os);

    os << ">\n";

    if (hasChildren()) {
      printChildren(os, hier);

      os << "</color-profile>\n";
    }
    else
      os << "/>\n";
  }
  else
    os << "color-profile";
}

void
CSVGColorProfile::
printValues(std::ostream &os, bool flat) const
{
  printNameValue(os, "local"           , local_);
  printNameValue(os, "name"            , name_);
  printNameValue(os, "rendering-intent", intent_);

  if (! xlink_.value().isNull())
    printNameXLink(os, "xlink:href", xlink_);

  CSVGObject::printValues(os, flat);
}
