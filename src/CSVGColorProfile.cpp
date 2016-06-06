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

    printNameValue(os, "local"           , local_);
    printNameValue(os, "name"            , name_);
    printNameValue(os, "rendering-intent", intent_);

    printNameXLink(os, "xlink:href", xlink_);

    CSVGObject::printValues(os);

    os << ">" << std::endl;

    if (hasChildren()) {
      printChildren(os, hier);

      os << "</color-profile>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "color-profile";
}

std::ostream &
operator<<(std::ostream &os, const CSVGColorProfile &c)
{
  c.print(os, false);

  return os;
}
