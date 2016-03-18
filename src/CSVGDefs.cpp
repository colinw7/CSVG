#include <CSVGI.h>

CSVGDefs::
CSVGDefs(CSVG &svg) :
 CSVGObject(svg),
 tokens_   ()
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
>
*/
bool
CSVGDefs::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGDefs::
draw()
{
}

void
CSVGDefs::
print(std::ostream &os) const
{
  os << "defs";
}

std::ostream &
operator<<(std::ostream &os, const CSVGDefs &defs)
{
  defs.print(os);

  return os;
}
