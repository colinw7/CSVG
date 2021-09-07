#include <CSVGAnchor.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGAnchor::
CSVGAnchor(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGAnchor::
CSVGAnchor(const CSVGAnchor &use) :
 CSVGObject(use),
 xlink_    (use.xlink_)
{
}

CSVGAnchor::
~CSVGAnchor()
{
}

CSVGAnchor *
CSVGAnchor::
dup() const
{
  return new CSVGAnchor(*this);
}

void
CSVGAnchor::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Presentation>
    <GraphicalEvents>
    <XLinkEmbed>
    <External>
*/
bool
CSVGAnchor::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if      (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else if (svg_.stringOption(opt_name, opt_value, "xlink:title", str))
    title_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGAnchor::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  return false;
}

void
CSVGAnchor::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<a";

    printValues(os);

    if (hasChildren()) {
      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</a>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "a";
}

void
CSVGAnchor::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  if (! xlink_.getValue().isNull())
    printNameXLink(os, "xlink:href", xlink_);
}
