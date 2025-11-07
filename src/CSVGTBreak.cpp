#include <CSVGTBreak.h>
#include <CSVG.h>
#include <CSVGLog.h>

/* Attributes:
    TODO
*/
CSVGTBreak::
CSVGTBreak(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGTBreak::
CSVGTBreak(const CSVGTBreak &tbreak) :
 CSVGObject(tbreak)
{
}

CSVGTBreak *
CSVGTBreak::
dup() const
{
  return new CSVGTBreak(*this);
}

bool
CSVGTBreak::
processOption(const std::string &, const std::string &)
{
  // TODO
  return true;
}

std::optional<std::string>
CSVGTBreak::
getNameValue(const std::string &name) const
{
  return CSVGObject::getNameValue(name);
}

bool
CSVGTBreak::
draw()
{
  return true;
}

void
CSVGTBreak::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<tbreak";

    printValues(os);

    os << ">";

    if (hasChildren()) {
      os << "\n";

      printChildren(os, hier);
    }
    else {
      os << getText();
    }

    os << "</tbreak>\n";
  }
  else
    os << "tbreak";
}

void
CSVGTBreak::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);
}
