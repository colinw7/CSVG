#include <CSVGSwitch.h>
#include <CSVG.h>

CSVGSwitch::
CSVGSwitch(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGSwitch::
CSVGSwitch(const CSVGSwitch &use) :
 CSVGObject(use)
{
}

CSVGSwitch::
~CSVGSwitch()
{
}

CSVGSwitch *
CSVGSwitch::
dup() const
{
  return new CSVGSwitch(*this);
}

bool
CSVGSwitch::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGSwitch::
draw()
{
}

void
CSVGSwitch::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<switch";

    CSVGObject::printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</switch>" << std::endl;
  }
  else
    os << "switch";
}

std::ostream &
operator<<(std::ostream &os, const CSVGSwitch &sw)
{
  sw.print(os, false);

  return os;
}
