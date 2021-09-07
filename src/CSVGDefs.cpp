#include <CSVGDefs.h>
#include <CSVG.h>

CSVGDefs::
CSVGDefs(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGDefs::
CSVGDefs(const CSVGDefs &defs) :
 CSVGObject(defs),
 tokens_   (defs.tokens_)
{
}

CSVGDefs *
CSVGDefs::
dup() const
{
  return new CSVGDefs(*this);
}

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Presentation>
    <GraphicalEvents>
    <External>
    transform <TransformList>
*/
bool
CSVGDefs::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGDefs::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    if (! objects_.empty()) {
      os << "<defs";

      printValues(os);

      os << ">" << std::endl;

      printChildren(os, hier);

      os << "</defs>" << std::endl;
    }
  }
  else
    os << "defs";
}

void
CSVGDefs::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);
}
