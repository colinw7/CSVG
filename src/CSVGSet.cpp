#include <CSVGSet.h>
#include <CSVG.h>
#include <CSVGLog.h>

/* Attributes:
    <Core>
    <Conditional>
    animation event attributes
    xlink attributes
    animation attribute target attributes
    animation timing attributes
    <External>
    'to'
*/

CSVGSet::
CSVGSet(CSVG &svg) :
 CSVGAnimateBase(svg)
{
}

CSVGSet::
CSVGSet(const CSVGSet &set) :
 CSVGAnimateBase(set)
{
}

CSVGSet *
CSVGSet::
dup() const
{
  return new CSVGSet(*this);
}

bool
CSVGSet::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (CSVGAnimateBase::processOption(opt_name, opt_value))
    return true;

  return false;
}

void
CSVGSet::
animate(double /*t*/)
{
  if (attributeName_.isValid() && to_.isValid()) {
    getParent()->processOption(getAttributeName(), getTo());

    svg_.redraw();
  }
}

void
CSVGSet::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<set";

    CSVGAnimateBase::printValues(os);

    os << "/>" << std::endl;
  }
  else
    os << "set ";
}
