#include <CSVGScript.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGScript::
CSVGScript(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGScript::
CSVGScript(const CSVGScript &use) :
 CSVGObject(use),
 type_     (use.type_)
{
}

CSVGScript::
~CSVGScript()
{
}

CSVGScript *
CSVGScript::
dup() const
{
  return new CSVGScript(*this);
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
CSVGScript::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if (svg_.stringOption(opt_name, opt_value, "type", str))
    type_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGScript::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;
}

void
CSVGScript::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<script";

    CSVGObject::printValues(os);

    printNameValue(os, "type", type_);

    if (hasText() || hasChildren()) {
      os << ">" << std::endl;

      if (hasText())
        os << getText() << std::endl;

      printChildren(os, hier);

      os << "</script>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "script";
}

std::ostream &
operator<<(std::ostream &os, const CSVGScript &use)
{
  use.print(os, false);

  return os;
}
