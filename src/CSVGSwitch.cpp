#include <CSVGI.h>

CSVGSwitch::
CSVGSwitch(CSVG &svg) :
 CSVGObject(svg)
{
}

bool
CSVGSwitch::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return true;
}

void
CSVGSwitch::
draw()
{
}

void
CSVGSwitch::
print(std::ostream &os) const
{
  os << "switch";
}

std::ostream &
operator<<(std::ostream &os, const CSVGSwitch &sw)
{
  sw.print(os);

  return os;
}
