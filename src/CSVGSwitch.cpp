#include <CSVGSwitch.h>
#include <CSVG.h>

CSVGSwitch::
CSVGSwitch(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGSwitch::
CSVGSwitch(const CSVGSwitch &s) :
 CSVGObject(s)
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

bool
CSVGSwitch::
draw()
{
  return false;
}

void
CSVGSwitch::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<switch";

    printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</switch>" << std::endl;
  }
  else
    os << "switch";
}

void
CSVGSwitch::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);
}
