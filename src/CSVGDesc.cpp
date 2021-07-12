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
  std::string str;

  if (svg_.stringOption(opt_name, opt_value, "role", str)) {
    if (str == "tooltip")
      tooltip_ = true;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGDesc::
draw()
{
  return false;
}

void
CSVGDesc::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<desc";

    printValues(os);

    os << ">";

    os << getText();

    printChildren(os, hier);

    os << "</desc>" << std::endl;
  }
  else
    os << "desc";
}

void
CSVGDesc::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);
}

std::ostream &
operator<<(std::ostream &os, const CSVGDesc &desc)
{
  desc.print(os, false);

  return os;
}
