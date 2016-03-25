#include <CSVGSwitch.h>
#include <CSVG.h>

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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<switch/>" << std::endl;
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
