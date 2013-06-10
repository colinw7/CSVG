#include "CSVGI.h"

CSVGSwitch::
CSVGSwitch(CSVG &svg) :
 CSVGObject(svg)
{
}

bool
CSVGSwitch::
processOption(const string &opt_name, const string &opt_value)
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
print(ostream &os) const
{
  os << "switch";
}

ostream &
operator<<(ostream &os, const CSVGSwitch &sw)
{
  sw.print(os);

  return os;
}
