#include <CSVGDesc.h>
#include <CSVG.h>

CSVGDesc::
CSVGDesc(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGDesc::
CSVGDesc(const CSVGDesc &desc) :
 CSVGObject(desc)
{
}

CSVGDesc *
CSVGDesc::
dup() const
{
  return new CSVGDesc(*this);
}

/* Attributes:
    <Core>
    <Style>
*/
bool
CSVGDesc::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGDesc::
draw()
{
}

void
CSVGDesc::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<desc";

    CSVGObject::printValues(os);

    os << ">";

    os << getText();

    printChildren(os, hier);

    os << "</desc>" << std::endl;
  }
  else
    os << "desc";
}

std::ostream &
operator<<(std::ostream &os, const CSVGDesc &desc)
{
  desc.print(os, false);

  return os;
}
